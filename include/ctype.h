/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <toolchain.h>

/**
 * Return whether `c` is an alphabetic character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is an alphabetic character, zero if not.
 */
int isalpha(int c);

/**
 * Return whether `c` is a control character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a control character, zero if not.
 */
int iscntrl(int c);

/**
 * Return whether `c` is a digit.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a digit, zero if not.
 */
int isdigit(int c);

/**
 * Return whether `c` is a printable character except space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a printable character and not space,
 *	zero if not.
 */
int isgraph(int c);

/**
 * Return whether `c` is a lowercase letter.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is lowercase, zero if not.
 */
int islower(int c);

/**
 * Return whether `c` is a printable character including space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a printable character, zero if not.
 */
int isprint(int c);

/**
 * Return whether `c` is a printable character
 * but neither alphanumeric nor space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a punctuation character, zero if not.
 */
int ispunct(int c);

/**
 * Return whether `c` is a white-space character.
 * These are: space, line-feed (`\n`), vertical tab (`\v`), form-feed (`\f`),
 * and carriage return (`\r`).
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a white-space character, zero if not.
 */
int isspace(int c);

/**
 * Return whether `c` is an uppercase letter.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is uppercase, zero if not.
 */
int isupper(int c);

/**
 * Return whether `c` is a hexadecimal digit.
 * This includes: 0-9, A-F and a-f.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a blaank character, zero if not.
 */
int isxdigit(int c);

/**
 * Return whether `c` is a 7-bit unsigned char.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is an ASCII character, zero if not.
 */
int isascii(int c);

/**
 * Return whether `c` is a space or a tab character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a blank character, zero if not.
 */
int isblank(int c);

/**
 * Check whether `c` is an alphanumeric character.
 * Equivalent to `(isalpha(c) || isdigit(c))`.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if the character is alphanumeric, zero if not.
 */
__always_inline int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
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
