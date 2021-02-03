/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/string.h>
#include <ardix/types.h>

#include <toolchain.h>

/*
 * Stupid memory allocator implementation.
 *
 * This design is heavily inspired by (read: stolen from) Doug Lea's Malloc
 * <http://gee.cs.oswego.edu/dl/html/malloc.html>.  It basically works like this:
 *
 * Memory is divided into individual blocks of dynamic size.  Every block has a header
 * containing its size w/out overhead; free blocks additionally have a
 * `struct list_head` after that in order to keep track of where the free blocks are.
 * This list is ordered by size ascendingly, so we can directly take the first
 * sufficiently-sized block when iterating over the list in `malloc()`.
 * Additionally, the effective block size is copied to the very end of the block
 * (directly after the last usable address) in order to detect two contiguous free
 * blocks when `free()`ing.  How?  By (ab)using the LSB of the at-least-4-byte-aligned
 * size value as a flag for whether the block is currently in use.  `free()` can then
 * just check the size values of the neighboring blocks by doing a simple pointer
 * calculation, and merge the two blocks into a big one if possible.  This minimizes
 * fragmentation with only slight overhead.
 *
 * On 32-bit systems, a free block in memory followed by an allocated one might look
 * something along the lines of this:
 *
 * -----------------------------------------------------------------------------
 * 0x20010000 | usable size in bytes (238)
 * 0x20010004 | ptr to next-smaller free block    \
 * 0x20010008 | ptr to next-bigger free block     | Usable memory area.  If this
 *      :     |                                   | was allocated, the returned
 *      :     |     <unused garbage data>         | ptr would be 0x20010004.
 *      :     |                                   /
 * 0x200100EE | usable size in bytes (238)
 * -----------------------------------------------------------------------------
 * 0x200100F2 | usable size in bytes (32) + 1 for the "used" bit
 *      :     |                                                    \
 *      :     |      <user-defined data>                           | 32 bytes
 *      :     |                                                    /
 * 0x20010112 | usable size in bytes (32 + 1)
 * -----------------------------------------------------------------------------
 *
 * That makes the minimal allocation size `sizeof(struct list_head *)`, because we need to
 * store those pointers for the linked list when `free()`ing a block.
 */

/**
 * Memory block header.
 * This sits at the beginning of every memory block (duh).
 */
struct memblk {
	/**
	 * The block's effectively usable size, i.e. the total block size minus
	 * `2 * MEMBLK_SIZE_LENGTH`.
	 *
	 * This size will also be written to the very end of the block, just after
	 * the last usable address.  Additionally, since blocks are always aligned
	 * to at least 4 bytes anyways, we can use the LSB of this size as a flag
	 * for whether the block is currently allocated (1) or not (0).  This is
	 * going to make it much easier to detect two free neighboring blocks when
	 * `free()`ing one.
	 */
	size_t size;

	/** If the block is allocated, this will be overwritten */
	struct list_head list;

	/* ... void ... */

	/* Here, at the end of this block, would be a copy of `size`. */
};

/** The length of the `size` member in `struct memblk`. */
#define MEMBLK_SIZE_LENGTH SIZEOF_MEMBER(struct memblk, size)
/** Total overhead per allocated block in bytes (2 * size_t). */
#define MEMBLK_OVERHEAD (2 * MEMBLK_SIZE_LENGTH)

/** Minimum effective allocation size (and all sizes must be a multiple of this one). */
#define MIN_BLKSZ (sizeof(struct memblk) - MEMBLK_OVERHEAD)

/** The list of free blocks, ordered by ascending size. */
LIST_HEAD(memblk_free_list);

static void memblk_set_size(struct memblk *block, size_t size)
{
	block->size = size;
	void *endptr = block;
	endptr += MEMBLK_SIZE_LENGTH;
	endptr += size & ~1u; /* discard the allocated bit */
	*(size_t *)(endptr) = size;
}

/**
 * Split a single free memory block up into two individual blocks such that the block
 * passed to this function will contain `size` bytes and the newly-created block has
 * the rest minus overhead.  The new block is inserted into the list of free blocks;
 * however, the original block will *not* be re-sorted.
 *
 * @param blk The block to split up.
 * @param size The new (at least by `MEMBLK_OFFSET + n` bytes smaller) size of the block.
 * @return The newly created block.
 */
static struct memblk *memblk_split(struct memblk *blk, size_t size)
{
	struct memblk *cursor;
	struct memblk *newblk = (void *)blk + MEMBLK_OVERHEAD + (size & ~1u);

	memblk_set_size(newblk, blk->size - MEMBLK_OVERHEAD - (size & ~1u));
	memblk_set_size(blk, size);

	list_for_each_entry_reverse(&blk->list, cursor, list) {
		if (cursor->size >= newblk->size || &cursor->list == &memblk_free_list) {
			list_insert(&cursor->list, &newblk->list);
			break;
		}
	}

	return newblk;
}

void malloc_init(void *heap, size_t size)
{
	struct memblk *blk = heap;

	/*
	 * TODO: This check will prevent accidentally calling the method twice, but should
	 *       ideally cause an error of some sort if it fails.  Once we have proper error
	 *       dispatching/handling routines, we should do that here.
	 */
	if (list_is_empty(&memblk_free_list)) {
		memset(heap, 0, size);
		memblk_set_size(blk, size - MEMBLK_OVERHEAD);
		list_insert(&memblk_free_list, &blk->list);
	}
}

__shared __attribute__((malloc))
void *malloc(size_t size)
{
	struct memblk *blk;
	size_t remaining_blksz;

	if (list_is_empty(&memblk_free_list))
		return NULL;

	if (size == 0)
		return NULL; /* as per POSIX.1-2008 */

	/* round up to the next multiple of `MIN_BLKSZ` */
	size = ((volatile)(size / MIN_BLKSZ)) * MIN_BLKSZ;
	size += MIN_BLKSZ;

	atomic_enter();

	list_for_each_entry(&memblk_free_list, blk, list) {
		/* blocks are sorted by size */
		if (blk->size >= size)
			break;
	}
	if (blk->size < size) {
		atomic_leave();
		return NULL; /* TODO: set errno to ENOMEM once we have it */
	}

	/*
	 * If we've made it to here, we have found a sufficiently big block,
	 * meaning we can't possibly fail anymore.  Since that block is likely
	 * larger than the requested size, we are going to check if it is
	 * possible to create a new, smaller block right at the end of the
	 * allocated area.  If it isn't, we just hand out the entire block.
	 */
	remaining_blksz = blk->size - size;
	if (remaining_blksz >= MIN_BLKSZ + MEMBLK_OVERHEAD)
		memblk_split(blk, size | 0x1u /* allocated bit */);
	else
		memblk_set_size(blk, blk->size | 0x1u /* allocated bit */);

	list_delete(&blk->list);

	atomic_leave();

	/* Keep the size field intact */
	return ((void *)blk) + MEMBLK_SIZE_LENGTH;
}

__shared __attribute__((malloc))
void *calloc(size_t nmemb, size_t size)
{
	void *ptr = malloc(nmemb * size);

	if (ptr != NULL)
		memset(ptr, 0, nmemb * size);

	return ptr;
}

/** Merge two neighboring free blocks to one big block */
static void memblk_merge(struct memblk *lblk, struct memblk *hblk)
{
	size_t *endsz = (void *)hblk + hblk->size + MEMBLK_SIZE_LENGTH;
	lblk->size = lblk->size + hblk->size + MEMBLK_OVERHEAD;
	*endsz = lblk->size;
}

__shared
void free(void *ptr)
{
	struct memblk *tmp;
	struct memblk *blk = ptr - MEMBLK_SIZE_LENGTH;
	size_t *neighsz;

	if (ptr == NULL)
		return; /* as per POSIX.1-2008 */

	if ((blk->size & 0x1u) == 0)
		return; /* TODO: Raise exception on double-free */

	atomic_enter();

	memblk_set_size(blk, blk->size & ~1u);

	/* check if our higher/right neighbor is allocated and merge if it is not */
	neighsz = (void *)blk + MEMBLK_OVERHEAD + blk->size;
	if ((*neighsz & 0x1u) == 0) {
		tmp = container_of(neighsz, struct memblk, size);
		memblk_merge(blk, tmp);
		list_delete(&tmp->list);
	}

	/* same thing for the lower/left block */
	neighsz = (void *)blk - MEMBLK_SIZE_LENGTH;
	if ((*neighsz & 0x1u) == 0) {
		tmp = (void *)neighsz - *neighsz - MEMBLK_SIZE_LENGTH;
		memblk_merge(tmp, blk);
		list_delete(&tmp->list);
		blk = tmp; /* discard the higher (now partial) block */
	}

	list_for_each_entry(&memblk_free_list, tmp, list) {
		if (tmp->size >= blk->size)
			break;
	}
	list_insert_before(&tmp->list, &blk->list);

	atomic_leave();
}

/*
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
