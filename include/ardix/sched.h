/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/list.h>
#include <ardix/types.h>

#ifndef CONFIG_SCHED_MAXTASK
/** The maximum number of tasks. */
#define CONFIG_SCHED_MAXTASK 8
#endif

#if CONFIG_SCHED_MAXTASK > 64
#warning "CONFIG_SCHED_MAXTASK is > 64, this could have a significant performance impact"
#endif

#ifndef CONFIG_SCHED_INTR_FREQ
/** Frequency (in Hertz) at which a scheduling interrupt should be fired */
#define CONFIG_SCHED_INTR_FREQ 10000U
#endif

#ifndef CONFIG_STACKSZ
/** Per-task stack size in bytes */
#define CONFIG_STACKSZ 4096U
#endif

enum task_state {
	/** Task is dead / doesn't exist */
	TASK_DEAD,
	/** Task is ready for execution or currently running. */
	TASK_READY,
	/** Task is waiting for its next time share. */
	TASK_QUEUE,
	/** Task is sleeping, `sleep_until` specifies when to wake it up. */
	TASK_SLEEP,
	/** Task is waiting for I/O to flush buffers. */
	TASK_IOWAIT,
};

/** Stores an entire process image. */
struct task {
	struct kent kent;
	/** current stack pointer (only gets updated for task switching) */
	void *sp;
	/** first address of the stack (highest if the stack grows downwards) */
	void *stack_bottom;
	/** if `state` is `TASK_SLEEP`, the last execution time */
	unsigned long int lastexec;
	/** if `state` is `TASK_SLEEP`, the amount of us to sleep in total */
	unsigned long int sleep_usecs;

	enum task_state state;
	pid_t pid;
};

/**
 * Initialize the scheduler subsystem.
 * This sets up a hardware interrupt timer (SysTick for Cortex-M3).
 */
int sched_init(void);

/**
 * Switch to the next task (interrupt context only).
 * Must be called directly from within an interrupt routine.
 * This selects a new task to be run and updates the old and new task's `state`
 * field to the appropriate value.
 *
 * @param curr_sp: stack pointer of the current task
 * @returns stack pointer of the new task
 */
void *sched_switch(void *curr_sp);

/**
 * Create a copy of the current process image and return it.
 *
 * @param task: the task to make a copy of
 * @returns the new (child) task copy, or `NULL` on failure
 */
struct task *sched_task_clone(struct task *task);

/**
 * Request the scheduler be invoked early, resulting in the current task to
 * be suspended.
 *
 * @param state: State the task should enter.
 *	Allowed values are `TASK_SLEEP` and `TASK_IOWAIT`.
 */
void sched_yield(enum task_state state);

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
