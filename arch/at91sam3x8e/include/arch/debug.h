/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <config.h>
#include <toolchain.h>

#if 1

#ifdef DEBUG
#	define __breakpoint __asm__ volatile("bkpt")
#else
#	define __breakpoint
#	define NDEBUG
#endif

#else
#define __breakpoint
#endif

__always_inline int __is_kernel(void) {
	int psr_val;

	__asm__ volatile(
"	mrs	%0,	psr	\n"
	: "=&r" (psr_val)
	);

	return psr_val & 0x01ff; /* bits 8-0 hold ISR_NUMBER */
}

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
