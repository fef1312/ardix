/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/atom.h>
#include <ardix/types.h>

/**
 * struct kent: Kernel Entity
 *
 * Kernel entities form a tree structure of reference counters and are meant to
 * be embedded into structures with dynamic lifetime.  The reference count is
 * incremented and decremented using the `kent_get()` and `kent_put()` functions
 * respectively, and when the count reaches zero the `destroy` callback is
 * invoked.  This callback is responsible for performing any cleanup work
 * required and releasing resources attached to the structure.  Additionally,
 * the parent kent's reference count is decremented as well.
 */
struct kent {
	struct kent *parent;
	atom_t refcount;
	void (*destroy)(struct kent *kent);
};

extern struct kent *kent_root;

/**
 * Initialize to root kent.
 * This should probably be called before bootstrapping anything else hooking
 * into the kent hierarchy i guess.
 *
 * @returns a nonzero value on failure
 */
int kent_root_init(void);

/**
 * Initialize a kent and set its refcount to one.
 * This will fail unless both the operations and parent members are initialized.
 * The parent refcount is incremented.
 *
 * @param kent: the kent
 * @returns a nonzero value on failure
 */
int kent_init(struct kent *kent);

/**
 * Increment the reference counter.
 *
 * @param kent: The kent.
 */
void kent_get(struct kent *kent);

/**
 * Decrement the reference counter.
 * If it reaches zero, the kent is destroyed by invoking the respective callback
 * in the operations field and the parent reference counter is also decremented.
 *
 * @param kent: The kent.
 */
void kent_put(struct kent *kent);

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
