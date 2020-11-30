/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

/*
 * An over-simplified approach of implementing locks on a system that
 * doesn't even support SMP.  Describing the realization as "unfortunate"
 * is pretty much an understatement, it is straight up horrible because it
 * can lead to two threads failing to acquire a lock at the same time
 * (but it is platform-agnostic thanks to C11, yay!).
 */

#pragma once

#include <errno.h>
#include <stdatomic.h>
#include <toolchain.h>

struct atom {
	atomic_int lock;
};

/** Initialize an atom to be used as a lock. */
__always_inline void atom_init(struct atom *atom)
{
	atom->lock = 0;
}

/**
 * Destroy this atom or fail if it is currently locked.
 * If successful, this will make any subsequent locking attempts fail.
 *
 * @param atom: The atom to be destroyed.
 * @returns 0 on success, and `-EAGAIN` if the atom is currently locked.
 */
__always_inline int atom_destroy(struct atom *atom)
{
	if (atom->lock != 0)
		return -EAGAIN;
	atom->lock = -1;
	return 0;
}

/**
 * Attempt to aquire a lock on an atom.
 *
 * @param atom: The atom to get the the lock on.
 * @returns 0 on success, and `-EAGAIN` if the atom was already locked by
 *	another process.
 */
__always_inline int atom_lock(struct atom *atom)
{
	atom->lock++;
	if (atom->lock != 1) {
		atom->lock--;
		return -EAGAIN;
	} else {
		return 0;
	}
}

/**
 * Release the lock on an atom.
 * Even though it is possible with the current implementation, releasing a
 * lock that isn't yours is a bad idea for obvious reasons.
 *
 * @param atom: The atom the release the lock from.
 */
__always_inline void atom_unlock(struct atom *atom)
{
	atom->lock--;
}

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
