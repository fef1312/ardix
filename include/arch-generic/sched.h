/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/hardware.h>

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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
