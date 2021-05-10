/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <toolchain.h>

/**
 * Return whether `c` is an alphabetic character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is an alphabetic character, zero if not.
 */
__const __shared int isalpha(int c);

/**
 * Return whether `c` is a control character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a control character, zero if not.
 */
__const __shared int iscntrl(int c);

/**
 * Return whether `c` is a digit.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a digit, zero if not.
 */
__const __shared int isdigit(int c);

/**
 * Return whether `c` is a printable character except space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a printable character and not space,
 *	zero if not.
 */
__const __shared int isgraph(int c);

/**
 * Return whether `c` is a lowercase letter.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is lowercase, zero if not.
 */
__const __shared int islower(int c);

/**
 * Return whether `c` is a printable character including space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a printable character, zero if not.
 */
__const __shared int isprint(int c);

/**
 * Return whether `c` is a printable character
 * but neither alphanumeric nor space.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a punctuation character, zero if not.
 */
__const __shared int ispunct(int c);

/**
 * Return whether `c` is a white-space character.
 * These are: space, line-feed (`\n`), vertical tab (`\v`), form-feed (`\f`),
 * and carriage return (`\r`).
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a white-space character, zero if not.
 */
__const __shared int isspace(int c);

/**
 * Return whether `c` is an uppercase letter.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is uppercase, zero if not.
 */
__const __shared int isupper(int c);

/**
 * Return whether `c` is a hexadecimal digit.
 * This includes: 0-9, A-F and a-f.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a blaank character, zero if not.
 */
__const __shared int isxdigit(int c);

/**
 * Return whether `c` is a 7-bit unsigned char.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is an ASCII character, zero if not.
 */
__const __shared int isascii(int c);

/**
 * Return whether `c` is a space or a tab character.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if `c` is a blank character, zero if not.
 */
__const __shared int isblank(int c);

/**
 * Check whether `c` is an alphanumeric character.
 * Equivalent to `(isalpha(c) || isdigit(c))`.
 *
 * @param c: The character, cast to an `int`.
 * @returns A nonzero value if the character is alphanumeric, zero if not.
 */
__const __shared int isalnum(int c);

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
