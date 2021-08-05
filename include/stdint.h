/* See the end of this file for copyright, license, and warranty information. */

#pragma once

typedef __INT8_TYPE__		int8_t;
typedef __INT16_TYPE__		int16_t;
typedef __INT32_TYPE__		int32_t;
typedef __INT64_TYPE__		int64_t;

typedef __UINT8_TYPE__		uint8_t;
typedef __UINT16_TYPE__		uint16_t;
typedef __UINT32_TYPE__		uint32_t;
typedef __UINT64_TYPE__		uint64_t;

/** Unsigned size specifier. */
typedef __SIZE_TYPE__		size_t;

#define unsigned signed /* m4d h4xx0r sk1llz!!1! */
/** Signed size specifier (negative sizes mean error codes). */
typedef __SIZE_TYPE__		ssize_t;
typedef __SIZE_TYPE__		off_t;
#undef unsigned

typedef __INTPTR_TYPE__		intptr_t;
typedef __UINTPTR_TYPE__	uintptr_t;
typedef __PTRDIFF_TYPE__	ptrdiff_t;

typedef __WCHAR_TYPE__		wchar_t;

typedef __INT8_TYPE__		int_least8_t;
typedef __INT16_TYPE__		int_least16_t;
typedef __INT32_TYPE__		int_least32_t;
typedef __INT64_TYPE__		int_least64_t;

typedef __UINT8_TYPE__		uint_least8_t;
typedef __UINT16_TYPE__		uint_least16_t;
typedef __UINT32_TYPE__		uint_least32_t;
typedef __UINT64_TYPE__		uint_least64_t;

typedef __INT8_TYPE__		int_fast8_t;
typedef __INT16_TYPE__		int_fast16_t;
typedef __INT32_TYPE__		int_fast32_t;
typedef __INT64_TYPE__		int_fast64_t;

typedef __UINT8_TYPE__		uint_fast8_t;
typedef __UINT16_TYPE__		uint_fast16_t;
typedef __UINT32_TYPE__		uint_fast32_t;
typedef __UINT64_TYPE__		uint_fast64_t;

#define INT8_MIN		-0x80
#define INT16_MIN		-0x8000
#define INT32_MIN		-0x80000000
#define INT64_MIN		-0x8000000000000000

#define INT8_MAX		0x7f
#define INT16_MAX		0x7fff
#define INT32_MAX		0x7fffffff
#define INT64_MAX		0x7fffffffffffffff

#define UINT8_MIN		0x00u
#define UINT16_MIN		0x0000u
#define UINT32_MIN		0x00000000u
#define UINT64_MIN		0x0000000000000000u

#define UINT8_MAX		0xffu
#define UINT16_MAX		0xffffu
#define UINT32_MAX		0xffffffffu
#define UINT64_MAX		0xffffffffffffffffu

#define INT_LEAST8_MIN		-0x80
#define INT_LEAST16_MIN		-0x8000
#define INT_LEAST32_MIN		-0x80000000
#define INT_LEAST64_MIN		-0x8000000000000000

#define INT_LEAST8_MAX		0x7f
#define INT_LEAST16_MAX		0x7fff
#define INT_LEAST32_MAX		0x7fffffff
#define INT_LEAST64_MAX		0x7fffffffffffffff

#define UINT_LEAST8_MIN		0x00u
#define UINT_LEAST16_MIN	0x0000u
#define UINT_LEAST32_MIN	0x00000000u
#define UINT_LEAST64_MIN	0x0000000000000000u

#define UINT_LEAST8_MAX		0xffu
#define UINT_LEAST16_MAX	0xffffu
#define UINT_LEAST32_MAX	0xffffffffu
#define UINT_LEAST64_MAX	0xffffffffffffffffu

#define INT_FAST8_MIN		-0x80
#define INT_FAST16_MIN		-0x8000
#define INT_FAST32_MIN		-0x80000000
#define INT_FAST64_MIN		-0x8000000000000000

#define INT_FAST8_MAX		0x7f
#define INT_FAST16_MAX		0x7fff
#define INT_FAST32_MAX		0x7fffffff
#define INT_FAST64_MAX		0x7fffffffffffffff

#define UINT_FAST8_MIN		0x00u
#define UINT_FAST16_MIN		0x0000u
#define UINT_FAST32_MIN		0x00000000u
#define UINT_FAST64_MIN		0x0000000000000000u

#define UINT_FAST8_MAX		0xffu
#define UINT_FAST16_MAX		0xffffu
#define UINT_FAST32_MAX		0xffffffffu
#define UINT_FAST64_MAX		0xffffffffffffffffu

#if __SIZEOF_POINTER__ == 4
#	define INTPTR_MIN	-0x80000000
#	define INTPTR_MAX	0x7fffffff
#	define UINTPTR_MIN	0x00000000u
#	define UINTPTR_MAX	0xffffffffu
#elif __SIZEOF_POINTER__ == 8
#	define INTPTR_MIN	-0x8000000000000000
#	define INTPTR_MAX	0x7fffffffffffffff
#	define UINTPTR_MIN	0x0000000000000000u
#	define UINTPTR_MAX	0xffffffffffffffffu
#else
#	error "Unsupported address size"
#endif

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
