/* See the end of this file for copyright, license, and warranty information. */

#include <arch/hardware.h>
#include <arch/interrupt.h>
#include <arch/linker.h>
#include <arch-generic/hardware.h>

#include <ardix/malloc.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

/* implementation in init/main.c */
extern int main(void);

__naked __noreturn void handle_reset(void)
{
	sys_init();

	memmove(&_srelocate, &_etext, (size_t)(&_erelocate) - (size_t)(&_srelocate));
	memset(&_szero, 0, (size_t)(&_ezero) - (size_t)(&_szero));

	for (uintptr_t *fn = &__preinit_array_start; fn != &__preinit_array_end; fn++)
		( (void (*)(void))*fn )();

	for (uintptr_t *fn = &__init_array_start; fn != &__init_array_end; fn++)
		( (void (*)(void))*fn )();

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
