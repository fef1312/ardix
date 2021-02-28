/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/atom.h>
#include <ardix/types.h>

struct kent;

/**
 * A collection of housekeeping callbacks for kents.
 * This is implemented by every module using kents.
 */
struct kent_ops {
	/**
	 * Destroy this kent and release all allocated resources.
	 * This is called when the refcount value reaches 0.
	 *
	 * @param kent: The kent to be destroyed.
	 */
	void (*destroy)(struct kent *kent);
};

/**
 * struct kent: Kernel Entity
 *
 * This is basically a primitive ripoff of the kobject system in Linux, except
 * there is no representation in a virtual filesystem and it is only really used
 * to keeping track of hierarchial reference counting.
 *
 * The main purpose of kents is to provide a basic common abstraction layer for
 * all modules and submodules of the Ardix kernel.  kents are arranged in a tree
 * structure, and use an atomic reference counter to keep track of when it is
 * safe to destroy them.  This structure is meant to be embedded into bigger
 * structs and then cast out of when a reference to this one is passed to one of
 * the
 */
struct kent {
	struct kent *parent;
	atom_t refcount;
	struct kent_ops *operations;
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
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
