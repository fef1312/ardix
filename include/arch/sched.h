/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

#pragma once

#include <arch/hardware.h>
#include <stdbool.h>

/**
 * Initialize a hardware timer for schduling.
 *
 * @param freq: The timer frequency in Hertz.
 */
int sched_hwtimer_init(unsigned int freq);

/**
 * Disable all scheduling interrupts in order to enter atomic context.
 */
void sched_atomic_enter(void);

/**
 * Re-enable scheduling interrupts, i.e. leave atomic context.
 *
 * @param resched: If `true`, request the scheduler to proceed to the next
 *	process ASAP.  Until then, put the CPU to sleep if required.
 */
void sched_atomic_leave(bool resched);

/**
 * Infinite loop of sleep instructions.
 */
void sched_idle_process_loop(void);

/**
 * Initialize the register values of a newly allocated process image.
 * Called by the scheduling subsystem when a process is being created.
 *
 * @param reg_snap: The stack memory location where the initial register values
 *	are to be loaded from.
 * @param entry: The process entry point.
 */
void sched_init_process_regs(struct reg_snapshot *reg_snap,
			     void (*entry)(void));
