/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch-generic/sched.h>

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
	/** Task is waiting for a mutex to be unlocked. */
	TASK_LOCKWAIT,
};

/** @brief Core structure holding information about a task. */
struct task {
	struct tcb tcb;

	struct kent kent;
	/**
	 * @brief Points to the bottom of the stack.
	 * In a full-descending stack, this is one word after the highest stack address.
	 */
	void *bottom;
	/** @brief If state is `TASK_SLEEP`, the total amount of ticks to sleep */
	unsigned long int sleep;
	/** @brief Last execution in ticks */
	unsigned long int last_tick;

	enum task_state state;
	pid_t pid;
};

/** @brief Current task (access from syscall context only) */
extern struct task *volatile current;

/** @brief Global system tick counter (may overflow) */
extern volatile unsigned long int tick;

/**
 * @brief Initialize the scheduler subsystem.
 * This sets up a hardware interrupt timer (SysTick for Cortex-M3).
 */
int sched_init(void);

/**
 * @brief Main scheduler routine.
 * This will iterate over the process table and choose a new task to be run,
 * which `current` is then updated to.  If the old task was in state
 * `TASK_READY`, it is set to `TASK_QUEUE`.
 */
void schedule(void);

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
 * @brief Invoke the scheduler early and switch tasks if required.
 * May only be called from syscall context.  Attention: If `state`
 * is `TASK_QUEUE`, this call is not guaranteed to suspend the
 * current task at all.
 *
 * @param state State the current task should enter.
 *	Allowed values are `TASK_QUEUE`, `TASK_SLEEP` and `TASK_IOWAIT`.
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
