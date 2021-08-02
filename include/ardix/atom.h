/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>
#include <toolchain.h>

#define ATOM_DEFINE(name) atom_t name = { .count = 0, }

void atom_init(atom_t *atom);

extern int _atom_get(int *count);
extern int _atom_put(int *count);

__always_inline int atom_get(atom_t *atom)
{
	return _atom_get(&atom->count);
}

__always_inline int atom_put(atom_t *atom)
{
	return _atom_put(&atom->count);
}

int atom_count(atom_t *atom);

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
