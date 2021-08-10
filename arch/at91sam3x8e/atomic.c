/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/atom.h>

static ATOM(atomic_context);

void atomic_enter(void)
{
	if (atom_get(&atomic_context) == 1)
		__asm__ volatile("cpsid i");
}

void atomic_leave(void)
{
	if (atom_put(&atomic_context) == 0)
		__asm__ volatile("cpsie i");
}

int is_atomic(void)
{
	return atom_count(&atomic_context);
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
