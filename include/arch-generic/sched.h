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
 * @brief Initialize a new task.
 *
 *
 * @param task Task to initialize
 * @param entry Task entry point
 */
void task_init(struct task *task, int (*entry)(void));

/** @brief Idle task entry point. */
__naked int _idle(void);

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
