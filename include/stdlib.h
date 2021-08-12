/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>
#include <toolchain.h>

/**
 * @defgroup malloc Memory Management
 *
 * @{
 */

/**
 * @brief Allocate `size` bytes of memory *w/out initializing it*.
 *
 * This method may block if an allocation is already taking place.
 * Use `atomickmalloc()` if you are in kernel space and in atomic context.
 *
 * @param size The amount of bytes to allocate.
 * @return A pointer to the beginning of the memory area, or `NULL` if
 *	`size` was 0 or there is not enough free memory left.
 */
__shared __malloc(free, 1) void *malloc(size_t size);

/**
 * @brief Allocate an array and initialize the memory to zeroes.
 * The allocated size will be at least `nmemb * size`.
 * If the multiplication would overflow, the allocation fails.
 *
 * @param nmemb The amount of members.
 * @param size The size of an individual member.
 * @return A pointer to the zeroed-out memory, or `NULL` if OOM.
 */
__malloc(free, 1) void *calloc(size_t nmemb, size_t size);

/**
 * @brief Free a previously allocated memory region.
 * Passing `NULL` has no effect.
 *
 * @param ptr The pointer, as returned by `malloc`/`calloc`.
 */
__shared void free(void *ptr);

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
