/* See the end of this file for copyright, license, and warranty information. */

#include <arch/hardware.h>
#include <arch/interrupt.h>
#include <arch-generic/hardware.h>

#include <ardix/malloc.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

/* from flash.ld */
extern uint32_t _sfixed;	/* fixed area start */
extern uint32_t _efixed;	/* fixed area end */
extern uint32_t _etext;		/* program (.text) end */
extern uint32_t _srelocate;	/* relocate (.data) start */
extern uint32_t _erelocate;	/* relocate end */
extern uint32_t _szero;		/* zero area (.bss) start */
extern uint32_t _ezero;		/* zero area end */
extern uint32_t _sstack;	/* stack start */
extern uint32_t _estack;	/* stack end */
extern uint32_t _sheap;		/* heap start */
extern uint32_t _eheap;		/* heap end */

/* implementation in init/main.c */
extern int main(void);

__naked __noreturn void handle_reset(void)
{
	sys_init();

	memmove(&_srelocate, &_etext, (size_t)(&_erelocate) - (size_t)(&_srelocate));
	memset(&_szero, 0, (size_t)(&_ezero) - (size_t)(&_szero));

	/* memory protection isn't implemented yet, heap is shared among all processes */
	malloc_init(&_sheap, (size_t)(&_eheap) - (size_t)(&_sheap));

	/* start the Kernel */
	main();

	/* halt (this should never be reached) */
	while (1);
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
