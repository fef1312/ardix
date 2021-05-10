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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
