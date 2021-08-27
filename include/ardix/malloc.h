/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>
#include <toolchain.h>

/**
 * @defgroup kmalloc Kernel Memory Management
 *
 * @{
 */

enum memflags {
	MEM_KERNEL	= (1 << 0),
	MEM_USER	= (1 << 1),
	MEM_ATOMIC	= (1 << 2),
	MEM_STACK	= (1 << 3),
};

/**
 * @brief Allocate `size` bytes of memory *w/out initializing it*.
 *
 * This method may block if an allocation is already taking place.
 * Use `atomic_kmalloc()` if you are in kernel space and in atomic context.
 *
 * @param size The amount of bytes to allocate.
 * @return A pointer to the beginning of the memory area, or `NULL` if
 *	`size` was 0 or there is not enough free memory left.
 */
__malloc(kfree, 1) void *kmalloc(size_t size, enum memflags flags);

/**
 * @brief Free a previously allocated memory region.
 * Passing `NULL` has no effect.
 *
 * @param ptr The pointer, as returned by `malloc`/`calloc`.
 */
void kfree(void *ptr);

/** Initialize the memory allocator, this is only called by the bootloader on early bootstrap. */
void kmalloc_init(void *heap, size_t size);

/** @} */

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
