/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>

#include <toolchain.h>

/**
 * Copy `n` bytes from `src` to `dest`.
 *
 * @param dest: The destination.
 * @param src: The source.
 * @param n: The amount of bytes to copy.
 * @returns A pointer to `dest`.
 */
__shared void *memcpy(void *dest, const void *src, size_t n);

/**
 * Starting from `ptr`, fill `n` bytes with the constant byte `c`.
 *
 * @param ptr: The start of the memory region.
 * @param c: The byte to fill with.
 * @param n: The amount of bytes to write.
 * @returns A pointer to `ptr`.
 */
__shared void *memset(void *ptr, int c, size_t n);

/**
 * Copy a memory area.
 * The two areas may overlap since the individual bytes are copied to a
 * temporary array first.
 *
 * @param dest: The destination address.
 * @param src: The source address.
 * @param n: The amount of bytes to copy.
 * @return a pointer to dest.
 */
__shared void *memmove(void *dest, const void *src, size_t n);

/**
 * Compare the two strings `s1` and `s2`.
 *
 * @param s1: The first string.
 * @param s2: The second string.
 * @returns `0` if both strings are equal, a positive value f `s1` is greater
 *	than `s2`, and a negative value if `s1` is less than `s2`.
 */
__shared int strcmp(const char *s1, const char *s2);

/**
 * Copy a `NUL` terminated string from `src` to `dest`.
 *
 * @param dest: The destination to copy to.  The memory region must be able to
 *	hold the entire string plus one byte for the terminator.
 * @param src: The original string to copy from.
 * @returns A pointer to the destination string.
 */
__shared char *strcpy(char *dest, const char *src);

/**
 * Copy a `NUL` terminated string from `src` to `dest`, but at most `n`
 * characters.  Note that this may cause `dest` to miss a `NUL` terminator.
 *
 * @param dest: The destination to copy to.  The memory region must be able to
 *	hold the entire string plus one byte for the terminator.
 * @param src: The original string to copy from.
 * @param n: The amount of characters to copy at most.
 * @returns A pointer to the destination string.
 */
__shared char *strncpy(char *dest, const char *src, size_t n);

/**
 * Compute the length of the `NUL` terminated string `s`.
 *
 * @param s: The string.
 * @returns The length of `s` without the `NUL` terminator.
 */
__shared size_t strlen(const char *s);

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
