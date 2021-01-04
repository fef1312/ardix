/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#ifndef __GNUC__
#error "Only GCC is supported"
#endif /* __GNUC__ */

#ifndef __always_inline
/**
 * Force a method to always be inlined by the compiler.
 * Do not use this for functions exceeding one or two lines.
 */
#define __always_inline inline __attribute__((always_inline))
#endif /* __always_inline */

#ifndef __naked
/** Function attribute for disabling register saving. */
#define __naked __attribute__((naked))
#endif

#ifndef __weak
/**
 * Add the `weak` attribute to a symbol.
 * This allows that identifier to be re-declared without any warnings.
 */
#define __weak __attribute__((__weak__))
#endif /* __weak */

#ifndef __alias
/**
 * Declare an identifier as an alias for some other identifier.
 *
 * @param name: The identifier (w/out quotes) this should be an alias for.
 */
#define __alias(name) __attribute__((alias(#name)))
#endif /* __alias */

#ifndef __section
/**
 * Define the program section this symbol should live in.
 *
 * @param name: The section name w/out quotes.
 */
#define __section(name) __attribute__((section(#name)))
#endif /* __section */

/** Place a variable in program memory rather than into RAM. */
#define __rodata __section(.rodata#)

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
