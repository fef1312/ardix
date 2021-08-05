/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#ifndef __GNUC__
#error "Only GCC is supported"
#endif /* __GNUC__ */

/** Force a method to always be inlined. */
#define __always_inline inline __attribute__(( always_inline ))

/** Function attribute for disabling register saving. */
#define __naked __attribute__(( naked ))

/** Function attribute denoting the call will never return. */
#define __noreturn __attribute__(( noreturn ))

/**
 * Add the `weak` attribute to a symbol.
 * This allows that identifier to be redeclared without any warnings.
 */
#define __weak __attribute__(( weak ))

/**
 * Declare an identifier as an alias for some other identifier.
 *
 * @param name: The identifier (w/out quotes) this should be an alias for.
 */
#define __alias(name) __attribute__(( alias(#name) ))

/**
 * Define the program section this symbol should live in.
 *
 * @param name: The section name w/out quotes.
 */
#define __section(name) __attribute__(( section(#name) ))

/** Place a variable in program memory rather than into RAM. */
#define __rodata __section(.rodata#)

/** Declare a function is pure so gcc can do some common subexpression elimination. */
#define __pure __attribute__(( pure ))

/** Like `__pure`, and the fuction does not access any memory except its stack. */
#define __const __attribute__(( __const__ ))

/** Denote a pointer to user space (this will be used for static code checks later)- */
#define __user

/** Storage attribute indicating the symbol will be shared with userspace. */
#define __shared __section(.text.shared)

/** Function attribute for hinting this function has malloc-like behavior. */
#define __malloc(deallocator, argn) __attribute__(( malloc ))

#define __preinit_call(fn) __section(.preinit_array) void (*fn##_ptr)(void) = fn

#define __init_call(fn) __section(.init_array) void (*fn##_ptr)(void) = fn

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
