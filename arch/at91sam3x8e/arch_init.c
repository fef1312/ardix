/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/malloc.h>
#include <toolchain.h>

/* flash.ld */
extern uint32_t _sheap;
extern uint32_t _eheap;

#include <arch/debug.h>

void __preinit_malloc(void)
{
	kmalloc_init(&_sheap, (size_t)&_eheap - (size_t)&_sheap);
}
__preinit_call(__preinit_malloc);

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
