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

/*
 * signed integer types
 */

#ifdef __INT8_TYPE__
typedef __INT8_TYPE__		int8_t;
#else
/** Signed 8-bit integer. */
typedef signed char		int8_t;
#endif /* __INT8_TYPE__ */

#ifdef __INT16_TYPE__
typedef __INT16_TYPE__		int16_t;
#else
/** Signed 16-bit integer. */
typedef signed int		int16_t;
#endif /* __INT16_TYPE__ */

#ifdef __INT32_TYPE__
typedef __INT32_TYPE__		int32_t;
#else
/** Signed 32-bit integer. */
typedef signed long int		int32_t;
#endif /* __INT32_TYPE__ */

/*
 * unsigned integer types
 */

#ifdef __UINT8_TYPE__
typedef __UINT8_TYPE__		uint8_t;
#else
/** Unsigned 8-bit integer. */
typedef unsigned char		uint8_t;
#endif /* __UINT8_TYPE__ */

#ifdef __UINT16_TYPE__
typedef __UINT16_TYPE__		uint16_t;
#else
/** Unsigned 16-bit integer. */
typedef unsigned int		uint16_t;
#endif /* __UINT16_TYPE__ */

#ifdef __UINT32_TYPE__
typedef __UINT32_TYPE__		uint32_t;
#else
/** Unsigned 32-bit integer. */
typedef unsigned long int	uint32_t;
#endif /* __UINT32_TYPE__ */

typedef uint8_t			bool;

/*
 * integer limits
 */

#define CHAR_MAX		((char)0x7f)
#define CHAR_MIN		(CHAR_MAX + 1)
#define UCHAR_MAX		((unsigned char)0xff)
#define UCHAR_MIN		((unsigned char)0x00)

#define INT_MAX			((int)0x7fff)
#define INT_MIN			(INT_MAX + 1)
#define UINT_MAX		((unsigned int)0xffff)
#define UINT_MIN		((unsigned int)0x0000)

#define LONG_MAX		((long)0x7fffffff)
#define LONG_MIN		(LONG_MAX + 1)
#define ULONG_MAX		((unsigned long)0xffffffff)
#define ULONG_MIN		((unsigned long)0x00000000)
