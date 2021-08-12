/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/hardware.h>

#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/util.h>

enum task_state {
	/** Task is dead / doesn't exist */
	TASK_DEAD,
	/** Task is currently running. */
	TASK_RUNNING,
	/** Task is waiting for its next time share. */
	TASK_QUEUE,
	/** Task is sleeping, `task::sleep` specifies for how many ticks. */
	TASK_SLEEP,
	/** Task is waiting for I/O to flush buffers. */
	TASK_IOWAIT,
	/** Task is waiting for a mutex to be unlocked. */
	TASK_LOCKWAIT,
	/** Task is waiting for child to  */
	TASK_WAITPID,
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
	/** @brief Lowest address in the stack, as returned by malloc. */
	void *stack;
	/** @brief If state is `TASK_SLEEP`, the total amount of ticks to sleep */
	unsigned long int sleep;
	/** @brief Last execution in ticks */
	unsigned long int last_tick;

	/*
	 * if a child process exited before its parent called waitpid(),
	 * this is where the children are stored temporarily
	 */
	struct list_head pending_sigchld;
	struct mutex pending_sigchld_lock;

	enum task_state state;
	pid_t pid;
};

__always_inline void task_get(struct task *task)
{
	kent_get(&task->kent);
}

__always_inline void task_put(struct task *task)
{
	kent_put(&task->kent);
}

__always_inline struct task *task_parent(struct task *task)
{
	if (task->pid == 0)
		return NULL;
	else
		return container_of(task->kent.parent, struct task, kent);
}

struct task_kevent {
	struct kevent kevent;
	struct task *task;
	int status;
};

void task_kevent_create_and_dispatch(struct task *task, int status);

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
