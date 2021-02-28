/* SPDX-License-Identifier: GPL-3.0-or-later */
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
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
