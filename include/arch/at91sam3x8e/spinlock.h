/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <arch/at91sam3x8e/spinlock_type.h>

#include <toolchain.h>

/* This code is basically stolen from arch/arm/include/asm/spinlock.h in Linux 5.9 */

#define SPINLOCK_DEFINE(name) spinlock_t name = { .lock = 0 }

/**
 * Initialize a spinlock.
 *
 * @param lock: Pointer to the spinlock.
 */
void arch_spinlock_init(spinlock_t *lock);

/**
 * Increment the lock count on a spinlock.
 *
 * @param lock: Pointer to the spinlock.
 * @returns The new lock count.
 */
int arch_spin_lock(spinlock_t *lock);

/**
 * Decrement the lock count on a spinlock.
 *
 * @param lock: Pointer to the spinlock.
 * @returns The new lock count.
 */
int arch_spin_unlock(spinlock_t *lock);

/**
 * Get the lock count on a spinlock.
 *
 * @param lock: Pointer to the spinlock.
 */
int arch_spinlock_count(spinlock_t *lock);

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
