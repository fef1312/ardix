/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#ifndef __GNUC__
#error "Only GCC is supported"
#endif /* __GNUC__ */

#ifndef __always_inline
/**
 * Force a method to always be inlined by the compiler.
 * Do not use this for functions exceeding one or two lines.
 */
#define __always_inline inline __attribute__(( always_inline ))
#endif /* __always_inline */

#ifndef __naked
/** Function attribute for disabling register saving. */
#define __naked __attribute__(( naked ))
#endif

#ifndef __noreturn
/** Function attribute denoting the call will never return. */
#define __noreturn __attribute__(( noreturn ))
#endif /* __noreturn */

#ifndef __weak
/**
 * Add the `weak` attribute to a symbol.
 * This allows that identifier to be re-declared without any warnings.
 */
#define __weak __attribute__(( weak ))
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

#ifndef __rodata
/** Place a variable in program memory rather than into RAM. */
#define __rodata __section(.rodata#)
#endif

#ifndef __pure
/** Declare a function is pure so gcc can do some common subexpression elimination. */
#define __pure __attribute__((pure))
#endif

#ifndef __const
/** Like `__pure`, and the fuction does not access any memory except its stack. */
#define __const __attribute__((const))
#endif

#ifndef __user
/** Denote a pointer to user space (this will be used for static code checks later) */
#define __user
#endif

#ifndef __shared
/** Storage attribute indicating the symbol will be shared with userspace. */
#define __shared __section(.text.shared)
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
