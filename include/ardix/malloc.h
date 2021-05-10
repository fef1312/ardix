/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>
#include <toolchain.h>

/**
 * Allocate `size` bytes of memory *w/out initializing it*.
 *
 * @param size The amount of bytes to allocate.
 * @return A pointer to the beginning of the memory area, or `NULL` if
 *	`size` was 0 or there is not enough free memory left.
 */
void *malloc(size_t size);

/**
 * Allocate an array and initialize the memory to zeroes.
 *
 * @param nmemb The amount of members.
 * @param size The size of an individual member.
 * @return A pointer to the zeroed-out memory, or `NULL` of `ENOMEM`.
 */
void *calloc(size_t nmemb, size_t size);

/**
 * Allocate at least `size` bytes of memory and initialize it to zero.
 *
 * @param size The amount of bytes to allocate.
 * @return A pointer to the beginning of the memory area, or `NULL` if
 *	`size` was 0 or there is not enough free memory left.
 */
__always_inline void *zalloc(size_t size)
{
	return calloc(1, size);
}

/**
 * Free a previously allocated memory region.
 * Passing `NULL` has no effect.
 *
 * @param ptr The pointer, as returned by `malloc`/`calloc`.
 */
void free(void *ptr);

/** Initialize the memory allocator, this is only called by the Kernel on early bootstrap. */
void malloc_init(void *heap, size_t size);

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
