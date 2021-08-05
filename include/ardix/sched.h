/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/list.h>
#include <ardix/types.h>

#include <config.h>

#if CONFIG_SCHED_MAXTASK > 64
#warning "CONFIG_SCHED_MAXTASK is > 64, this could have a significant performance impact"
#endif

enum task_state {
	/** Task is dead / doesn't exist */
	TASK_DEAD,
	/** Task is ready for execution or currently running. */
	TASK_READY,
	/** Task is waiting for its next time share. */
	TASK_QUEUE,
	/** Task is sleeping, `task::sleep` specifies for how many ticks. */
	TASK_SLEEP,
	/** Task is waiting for I/O to flush buffers. */
	TASK_IOWAIT,
};

/** @brief Core structure holding information about a task. */
struct task {
	struct kent kent;
	/** current stack pointer (only gets updated for task switching) */
	void *sp;
	/** first address of the stack (highest if the stack grows downwards) */
	void *stack_bottom;
	/** @brief If state is `TASK_SLEEP`, the total amount of ticks to sleep */
	unsigned long int sleep;
	/** @brief Last execution in ticks */
	unsigned long int last_tick;

	enum task_state state;
	pid_t pid;
};

/** @brief Current task (access from syscall context only) */
extern struct task *current;

/** @brief Global system tick counter (may overflow) */
extern volatile unsigned long int tick;

/**
 * @brief If nonzero, the scheduler is invoked after the current syscall.
 * This is checked and then cleared after every syscall.  If it has a nonzero
 * value, `sched_switch()` is called after `arch_enter()`.
 */
extern int need_resched;

/**
 * @brief Initialize the scheduler subsystem.
 * This sets up a hardware interrupt timer (SysTick for Cortex-M3).
 */
int sched_init(void);

/**
 * @brief Switch to the next task (scheduler context only).
 * Must be called directly from within an interrupt routine.
 * This selects a new task to be run and updates the old and new task's `state`
 * field to the appropriate value.  Called from the scheduler exception handler.
 *
 * @param curr_sp Stack pointer of the current task
 * @returns Stack pointer of the new task
 */
void *sched_switch(void *curr_sp);

/**
 * @brief Create a copy of the `current` task and return it.
 * The new task becomes a child of the `current` task and is inserted into the
 * process table so that it can be executed by the scheduler after its state
 * is set to `TASK_QUEUE`.  When the task is returned, its initial state is
 * `TASK_UNKNOWN` so that the caller has time to do any additional required
 * setup work.
 *
 * @param task Task to make a copy of
 * @returns The new (child) task copy, or `NULL` on failure
 */
struct task *task_clone(struct task *task);

/**
 * @brief Sleep for an approximate amount of milliseconds.
 * Must not be invoked from atomic or irq context.
 *
 * @param ms Amount of milliseconds
 */
void msleep(unsigned long int ms);

/**
 * @brief Suspend the `current` task and invoke the scheduler early.
 * May only be called from syscall context.
 *
 * @param state State the task should enter.
 *	Allowed values are `TASK_SLEEP` and `TASK_IOWAIT`.
 */
void yield(enum task_state state);

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
