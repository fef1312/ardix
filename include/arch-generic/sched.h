/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/hardware.h>

#include <toolchain.h>

struct task; /* see include/ardix/sched.h */

/**
 * Initialize a hardware timer for schduling.
 *
 * @param freq: The timer frequency in Hertz.
 */
int arch_sched_init(unsigned int freq);

/**
 * Initialize a new process.
 * This requires the process' `stack_base` field to be initialized as the
 * initial register values are written to the stack.
 *
 * @param process: The process.
 * @param entry: The process entry point.
 */
void arch_task_init(struct task *task, void (*entry)(void));

int arch_idle_task_init(struct task *task);

/**
 * @brief Convert milliseconds to system ticks, rounding to zero.
 *
 * @param ms Amount of milliseconds
 * @returns Equivalent time in system ticks
 */
unsigned long int ms_to_ticks(unsigned long ms);

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
