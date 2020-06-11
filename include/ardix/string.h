/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

#pragma once

#include <ardix/types.h>

int memcmp(const void *s1, const void *s2, size_t n);

/**
 * Copy `n` bytes from `src` to `dest`.
 *
 * @param dest: The destination.
 * @param src: The source.
 * @param n: The amount of bytes to copy.
 * @returns A pointer to `dest`.
 */
void *memcpy(void *dest, const void *src, size_t n);

/**
 * Starting from `ptr`, fill `n` bytes with the constant byte `c`.
 *
 * @param ptr: The start of the memory region.
 * @param c: The byte to fill with.
 * @param n: The amount of bytes to write.
 * @returns A pointer to `ptr`.
 */
void *memset(void *ptr, int c, size_t n);

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
void *memmove(void *dest, const void *src, size_t n);

/**
 * Compare the two strings `s1` and `s2`.
 *
 * @param s1: The first string.
 * @param s2: The second string.
 * @returns `0` if both strings are equal, a positive value f `s1` is greater
 *	than `s2`, and a negative value if `s1` is less than `s2`.
 */
int strcmp(const char *s1, const char *s2);

/**
 * Copy a `NUL` terminated string from `src` to `dest`.
 *
 * @param dest: The destination to copy to.  The memory region must be able to
 *	hold the entire string plus one byte for the terminator.
 * @param src: The original string to copy from.
 * @returns A pointer to the destination string.
 */
char *strcpy(char *dest, const char *src);

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
char *strncpy(char *dest, const char *src, size_t n);

/**
 * Compute the length of the `NUL` terminated string `s`.
 *
 * @param s: The string.
 * @returns The length of `s` without the `NUL` terminator.
 */
size_t strlen(const char *s);
