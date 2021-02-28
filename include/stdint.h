/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

typedef __INT8_TYPE__		int8_t;
typedef __INT16_TYPE__		int16_t;
typedef __INT32_TYPE__		int32_t;

#ifdef __INT64_TYPE__
typedef __INT64_TYPE__		int64_t;
#endif

typedef __UINT8_TYPE__		uint8_t;
typedef __UINT16_TYPE__		uint16_t;
typedef __UINT32_TYPE__		uint32_t;

#ifdef __UINT64_TYPE__
typedef __UINT64_TYPE__		uint64_t;
#endif

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned long int
#endif /* __SIZE_TYPE__ */

/** Unsigned size specifier. */
typedef __SIZE_TYPE__		size_t;
typedef __SIZE_TYPE__		uintptr_t;
/** Signed size specifier (negative sizes mean error codes). */
typedef __PTRDIFF_TYPE__	ssize_t;
typedef __PTRDIFF_TYPE__	ptrdiff_t;
typedef __PTRDIFF_TYPE__	off_t;
typedef long __PTRDIFF_TYPE__	loff_t;

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
