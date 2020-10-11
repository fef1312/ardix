/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#define CHAR_MAX		__SCHAR_MAX__
#define CHAR_MIN		(CHAR_MAX + 1)
#define UCHAR_MAX		((unsigned char)0 - 1)
#define UCHAR_MIN		((unsigned char)0)

#define SHRT_MAX		__SHRT_MAX__
#define SHRT_MIN		(SHRT_MAX + 1)
#define USHRT_MAX		((unsigned short)0 - 1)
#define USHRT_MIN		((unsigned short)0)

#define INT_MAX			__INT_MAX__
#define INT_MIN			(INT_MAX + 1)
#define UINT_MAX		((unsigned int)0 - 1)
#define UINT_MIN		((unsigned int)0)

#define LONG_MAX		__LONG_MAX__
#define LONG_MIN		(LONG_MAX + 1)
#define ULONG_MAX		((unsigned long)0 - 1)
#define ULONG_MIN		((unsigned long)0)

#define LLONG_MAX		__LLONG_MAX__
#define LLONG_MIN		(LLONG_MAX + 1)
#define ULLONG_MAX		((unsigned long long)0 - 1)
#define ULLONG_MIN		((unsigned long long)0)

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
