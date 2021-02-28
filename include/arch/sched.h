/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/hardware.h>

#include <stdbool.h>
#include <toolchain.h>

struct process; /* see include/ardix/sched.h */

/**
 * Initialize a hardware timer for schduling.
 *
 * @param freq: The timer frequency in Hertz.
 */
int arch_sched_hwtimer_init(unsigned int freq);

/**
 * Initialize a new process.
 * This requires the process' `stack_base` field to be initialized as the
 * initial register values are written to the stack.
 *
 * @param process: The process.
 * @param entry: The process entry point.
 */
void arch_sched_process_init(struct process *process, void (*entry)(void));

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
