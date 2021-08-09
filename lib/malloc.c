/* See the end of this file for copyright, license, and warranty information. */

#include <arch/debug.h>

#include <ardix/atomic.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/mutex.h>
#include <ardix/types.h>

#include <string.h>
#include <toolchain.h>
#include <config.h>

/**
 * @file Stupid memory allocator.
 *
 * This implementation is originally based on Doug Lea's design
 * <http://gee.cs.oswego.edu/dl/html/malloc.html>, with some features (notably
 * binning) removed for the sake of simplicity.  Furthermore, as the MPU is not
 * implemented yet, the allocator uses only two heaps: one for all regular
 * processes including the kernel, and one for timing critical situations where
 * we can't sleep (mainly irqs).  Additionally, there is no wilderness chunk to
 * take care of because we don't support virtual memory.
 *
 * Memory is divided into individual blocks of dynamic size.  Every block has a
 * header containing its size w/out overhead; free blocks additionally have a
 * `struct list_head` after that in order to keep track of where the free blocks
 * are.  This list is ordered by size ascendingly, so we can directly take the
 * first sufficiently sized block when iterating over the list in `malloc()`.
 *
 * Additionally, the effective block size is copied to the very end of the block
 * (directly after the last usable address) in order to be able to find a
 * block's immediate neighbors by simple pointer arithmetic.  For this to work,
 * the allocator relies on the fact that blocks are always aligned to at least
 * one longword.  With that in mind, we know that the two LSBs of the size are
 * always zero (longwords must be at least 4 bytes as per the C standard) and
 * thus can be (ab)used for flags.  The size at the beginning of the block will
 * be referred to as the lower size, and the copy at the end of the block is the
 * upper size.
 *
 * Bit 0 flags the block as either free or allocated.  If two contiguous blocks
 * become free, they are merged back together into one large block.  This flag
 * is always the same at both the upper and lower size.
 *
 * Bit 1 is a marker for the start and end of the heap.  The lowest block, i.e.
 * the one with the lowest physical memory address, has this flag set in its
 * lower size field, but not in the upper one.  Likewise, the highest block,
 * i.e. the one with the highest physical memory address, has this flag set in
 * its upper size field, but not in the lower one.  All other blocks always set
 * this flag to zero in both size fields.  In other words, this flag is set in
 * the block's upper/lower size field if if does *not* have an upper/lower
 * neighbor respectively.
 *
 * On 32-bit systems, a free block in memory followed by an allocated one might look
 * something along the lines of this:
 *
 * ~~~{.txt}
 * -----------------------------------------------------------------------------
 * 0x20010000 | usable size in bytes (236)
 * 0x20010004 | ptr to next smaller free block    \
 * 0x20010008 | ptr to next bigger free block     | Usable memory area.  If this
 *      :     |                                   | was allocated, the returned
 *      :     |     <unused garbage data>         | ptr would be 0x20010004.
 *      :     |                                   /
 * 0x200100EC | usable size in bytes (236)
 * -----------------------------------------------------------------------------
 * 0x200100F0 | usable size in bytes (32) + 1 for the "used" bit
 *      :     |                                                    \
 *      :     |      <user data>                                   | 32 bytes
 *      :     |                                                    /
 * 0x20010110 | usable size in bytes (32 + 1)
 * -----------------------------------------------------------------------------
 * ~~~
 *
 * That makes the minimal allocation size `sizeof(struct list_head *)`, because we need to
 * store those pointers for the linked list when `free()`ing a block.
 */

#if __SIZEOF_SIZE_T__ < 4
#error "size_t must be at least 4 bytes"
#endif

/**
 * Memory block header.
 * This sits at the beginning of every memory block (duh).
 */
struct memblk {
	union {
		/**
		 * @brief The usable size, i.e. the total block size minus `MEMBLK_OVERHEAD`.
		 *
		 * This size will also be written to the very end of the block, just after
		 * the last usable address.  Additionally, since blocks are always aligned
		 * to at least 4 bytes anyways, we can use the LSB of this size as a flag
		 * for whether the block is currently allocated (1) or not (0).  This is
		 * going to make it much easier to detect two free neighboring blocks when
		 * `free()`ing one.
		 */
		size_t size;
		/** @brief Used to get the previous block's size by accessing index -1 */
		size_t prevsz[0];
	};

	union {
		/** @brief If the block is allocated, this will be overwritten */
		struct list_head list;

		/** @brief Used as the return value for `malloc()` */
		uint8_t data[0];
		/** @brief Used to get the copy of the size field at the end of the block */
		size_t endsz[0];
	};
};

#define OVERHEAD (2 * SIZEOF_MEMBER(struct memblk, size))
#define MIN_SIZE SIZEOF_MEMBER(struct memblk, list)

static LIST_HEAD(generic_heap);
static MUTEX(generic_heap_lock);
static void *generic_heap_start;
static void *generic_heap_end;

static LIST_HEAD(atomic_heap);
static void *atomic_heap_start;
static void *atomic_heap_end;

/* forward declaration of utility functions used throughout the file */

/** @brief Get the usable block size in bytes, without flags or overhead. */
static size_t blk_get_size(struct memblk *blk);
/** @brief Set the usable block size without overhead and without affecting flags. */
static void blk_set_size(struct memblk *blk, size_t size);
/** @brief Flag a block as allocated. */
static void blk_set_alloc(struct memblk *blk);
/** @brief Remove the allocated flag from a block. */
static void blk_clear_alloc(struct memblk *blk);
/** @brief Return nonzero if the block is allocated. */
static int blk_is_alloc(struct memblk *blk);
/** @brief Set the border flag at the start of a block. */
static void blk_set_border_start(struct memblk *blk);
/** @brief Remove the border flag from the start of a block. */
static void blk_clear_border_start(struct memblk *blk);
/** @brief Return nonzero if a block has the border flag set at the start. */
static int blk_is_border_start(struct memblk *blk);
/** @brief Set the border flag at the end of a block. */
static void blk_set_border_end(struct memblk *blk);
/** @brief Remove the border flag from the end of a block. */
static void blk_clear_border_end(struct memblk *blk);
/** @brief Return nonzero if a block has the border flag set at the end. */
static int blk_is_border_end(struct memblk *blk);
/** @brief Get a block's immediate lower neighbor, or NULL if it doesn't have one. */
static struct memblk *blk_prev(struct memblk *blk);
/** @brief Get a block's immediate higher neighbor, or NULL if it doesn't have one. */
static struct memblk *blk_next(struct memblk *blk);
/** @brief Merge two contiguous free blocks into one, resort the list, and return the block. */
static struct memblk *blk_merge(struct list_head *heap, struct memblk *bottom, struct memblk *top);
/** @brief Attempt to merge both the lower and higher neighbors of a free block. */
static struct memblk *blk_try_merge(struct list_head *heap, struct memblk *blk);
/** @brief Cut a slice from a free block and return the slice. */
static struct memblk *blk_slice(struct list_head *heap, struct memblk *bottom, size_t bottom_size);

void malloc_init(void *heap, size_t size)
{
#	ifdef DEBUG
		if (heap == NULL) {
			__breakpoint;
		}
		if (size == 0) {
			__breakpoint;
		}
		if (size - OVERHEAD - MIN_SIZE < CONFIG_IOMEM_SIZE) {
			__breakpoint;
		}
		if (!list_is_empty(&generic_heap)) {
			__breakpoint;
		}
		if (!list_is_empty(&atomic_heap)) {
			__breakpoint;
		}
#	endif

	memset(heap, 0, size);

	generic_heap_start = heap;
	generic_heap_end = heap + size - CONFIG_IOMEM_SIZE - OVERHEAD;

	atomic_heap_start = heap + size - CONFIG_IOMEM_SIZE;
	atomic_heap_end = atomic_heap_start + CONFIG_IOMEM_SIZE;

	struct memblk *generic_block = heap;
	blk_set_size(generic_block, size - CONFIG_IOMEM_SIZE - OVERHEAD);
	blk_clear_alloc(generic_block);
	blk_set_border_start(generic_block);
	blk_set_border_end(generic_block);
	list_insert(&generic_heap, &generic_block->list);

	struct memblk *atomic_block = heap + size - CONFIG_IOMEM_SIZE;
	blk_set_size(atomic_block, CONFIG_IOMEM_SIZE - OVERHEAD);
	blk_clear_alloc(atomic_block);
	blk_set_border_start(atomic_block);
	blk_set_border_end(atomic_block);
	list_insert(&atomic_heap, &atomic_block->list);
}

void *malloc(size_t size)
{
	if (size == 0)
		return NULL; /* as per POSIX */

	/*
	 * Round up towards the next whole allocation unit.  GCC is smart enough
	 * to replace the division/multiplication pair with a bitfield clear
	 * instruction (MIN_SIZE is always a power of two), so this is okay.
	 */
	size = (size / MIN_SIZE) * MIN_SIZE + MIN_SIZE;

	mutex_lock(&generic_heap_lock);

	struct memblk *cursor;
	list_for_each_entry(&generic_heap, cursor, list) {
		if (blk_get_size(cursor) >= size)
			break;
	}

	void *ptr = NULL;

	if (blk_get_size(cursor) >= size) {
		cursor = blk_slice(&generic_heap, cursor, size);
		ptr = cursor->data;
	}

	mutex_unlock(&generic_heap_lock);

	return ptr;
}

void *atomic_malloc(size_t size)
{
	if (size == 0)
		return NULL;

	size = (size / MIN_SIZE) * MIN_SIZE + MIN_SIZE;

	atomic_enter();

	struct memblk *cursor;
	list_for_each_entry(&atomic_heap, cursor, list) {
		if (blk_get_size(cursor) >= size)
			break;
	}

	void *ptr = NULL;

	if (blk_get_size(cursor) >= size) {
		cursor = blk_slice(&atomic_heap, cursor, size);
		ptr = cursor->data;
	}

	atomic_leave();

	return ptr;
}

void *calloc(size_t nmemb, size_t size)
{
	size_t total = nmemb * size;

	/* check for overflow as mandated by POSIX */
	if (size != 0 && total / size != nmemb)
		return NULL;

	void *ptr = malloc(total);

	if (ptr != NULL)
		memset(ptr, 0, total);

	return ptr;
}

void free(void *ptr)
{
	if (ptr == NULL)
		return; /* as per POSIX.1-2008 */

	struct memblk *blk = ptr - offsetof(struct memblk, data);

	if (ptr >= generic_heap_start && ptr <= generic_heap_end) {
		if (!blk_is_alloc(blk))
			__breakpoint;

		mutex_lock(&generic_heap_lock);
		blk_clear_alloc(blk);
		blk_try_merge(&generic_heap, blk);
		mutex_unlock(&generic_heap_lock);
	} else if (ptr >= atomic_heap_start && ptr <= atomic_heap_end) {
		if (!blk_is_alloc(blk))
			__breakpoint;

		atomic_enter();
		blk_clear_alloc(blk);
		blk_try_merge(&atomic_heap, blk);
		atomic_leave();
	} else {
		__breakpoint;
	}
}

/* ========================================================================== */

/*
 * The rest of this file is just the utility functions that make our life a
 * little easier.  Nothing too spectacular going on here, everything should be
 * obvious from reading the huge comment at the top.
 */

#define ALLOC_FLAG	((size_t)1 << 0)
#define BORDER_FLAG	((size_t)1 << 1)
#define SIZE_MSK	( ~(ALLOC_FLAG | BORDER_FLAG) )

static inline struct memblk *blk_try_merge(struct list_head *heap, struct memblk *blk)
{
	struct memblk *neighbor = blk_prev(blk);
	if (neighbor != NULL && !blk_is_alloc(neighbor)) {
		list_delete(&neighbor->list);
		blk = blk_merge(heap, neighbor, blk);
	}

	neighbor = blk_next(blk);
	if (neighbor != NULL && !blk_is_alloc(neighbor)) {
		list_delete(&neighbor->list);
		blk = blk_merge(heap, blk, neighbor);
	}

	struct memblk *cursor;
	list_for_each_entry(heap, cursor, list) {
		if (blk_get_size(cursor) >= blk_get_size(blk))
			break;
	}
	list_insert_before(&cursor->list, &blk->list);

	return blk;
}

static inline struct memblk *blk_merge(struct list_head *heap,
				       struct memblk *bottom,
				       struct memblk *top)
{
	size_t bottom_size = blk_get_size(bottom);
	size_t top_size = blk_get_size(top);
	size_t total_size = bottom_size + top_size + OVERHEAD;

	blk_set_size(bottom, total_size);

	return bottom;
}

static struct memblk *blk_slice(struct list_head *heap, struct memblk *blk, size_t slice_size)
{
	list_delete(&blk->list);

	assert((slice_size & SIZE_MSK) == slice_size);
	assert(slice_size > 0);

	size_t rest_size = blk_get_size(blk) - slice_size - OVERHEAD;
	if (rest_size < MIN_SIZE) {
		blk_set_alloc(blk);
		return blk; /* hand out the entire block */
	}

	size_t slice_words = slice_size / sizeof(blk->size);
	struct memblk *rest = (void *)&blk->endsz[slice_words + 1];
	blk_set_size(rest, rest_size);
	blk_clear_alloc(rest);
	blk_clear_border_start(rest);

	blk_set_size(blk, slice_size);
	blk_set_alloc(blk);
	blk_clear_border_end(blk);

	struct memblk *cursor;
	list_for_each_entry(heap, cursor, list) {
		if (blk_get_size(cursor) <= rest_size)
			break;
	}
	list_insert_before(&cursor->list, &rest->list);

	return blk;
}

static inline size_t blk_get_size(struct memblk *blk)
{
	return blk->size & SIZE_MSK;
}

static void blk_set_size(struct memblk *blk, size_t size)
{
	assert((size & SIZE_MSK) == size);

	/* don't affect flags */

	blk->size &= ~SIZE_MSK;
	blk->size |= size;

	size_t words = size / sizeof(blk->size);
	blk->endsz[words] &= ~SIZE_MSK;
	blk->endsz[words] |= size;
}

static inline void blk_set_alloc(struct memblk *blk)
{
	size_t words = blk->size / sizeof(blk->size);

	blk->size |= ALLOC_FLAG;
	blk->endsz[words] |= ALLOC_FLAG;
}

static inline void blk_clear_alloc(struct memblk *blk)
{
	size_t words = blk->size / sizeof(blk->size);

	blk->size &= ~ALLOC_FLAG;
	blk->endsz[words] &= ~ALLOC_FLAG;
}

static inline int blk_is_alloc(struct memblk *blk)
{
	return blk->size & ALLOC_FLAG;
}

static inline void blk_set_border_start(struct memblk *blk)
{
	blk->size |= BORDER_FLAG;
}

static inline void blk_clear_border_start(struct memblk *blk)
{
	blk->size &= ~BORDER_FLAG;
}

static inline int blk_is_border_start(struct memblk *blk)
{
	return blk->size & BORDER_FLAG;
}

static inline void blk_set_border_end(struct memblk *blk)
{
	size_t words = blk->size / sizeof(blk->size);
	blk->endsz[words] |= BORDER_FLAG;
}

static inline void blk_clear_border_end(struct memblk *blk)
{
	size_t words = blk->size / sizeof(blk->size);
	blk->endsz[words] &= ~BORDER_FLAG;
}

static inline int blk_is_border_end(struct memblk *blk)
{
	size_t words = blk->size / sizeof(blk->size);
	return blk->endsz[words] & BORDER_FLAG;
}

static inline struct memblk *blk_prev(struct memblk *blk)
{
	if (blk_is_border_start(blk))
		return NULL;
	return (void *)blk - blk->prevsz[-1] - OVERHEAD;
}

static inline struct memblk *blk_next(struct memblk *blk)
{
	if (blk_is_border_end(blk))
		return NULL;

	size_t words = blk->size / sizeof(blk->size);
	return (void *)&blk->endsz[words + 1];
}

/*
 * This file is part of Ardix.
 * Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
