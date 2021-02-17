/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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
 * This is basically a primitive ripoff of the kobject system in Linux.
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
 * Initialize a kent and increment its refcounter by one.
 *
 * @param kent: The kent.
 * @param parent: The parent kent.
 * @returns A nonzero value on failure
 */
int kent_init(struct kent *kent, struct kent *parent);

/**
 * Increment the reference counter.
 *
 * @param kent: The kent.
 */
void kent_get(struct kent *kent);

/**
 * Decrement the reference counter.
 * If it reaches zero, the kent is destroyed by invoking the respective callback
 * in the operations field.
 *
 * @param kent: The kent.
 */
void kent_put(struct kent *kent);

/*
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
