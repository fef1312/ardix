/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/sched.h>
#include <arch-generic/watchdog.h>

#include <ardix/atomic.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>
#include <string.h>

extern uint32_t _sstack;
extern uint32_t _estack;

static struct task *_sched_tasktab[CONFIG_SCHED_MAXTASK];
struct task *_sched_current_task;

static void task_destroy(struct kent *kent)
{
	struct task *task = container_of(kent, struct task, kent);
	_sched_tasktab[task->pid] = NULL;
	free(task);
}

int sched_init(void)
{
	int i;

	_sched_current_task = malloc(sizeof(*_sched_current_task));
	if (_sched_current_task == NULL)
		return -ENOMEM;

	_sched_current_task->kent.parent = kent_root;
	_sched_current_task->kent.destroy = task_destroy;
	i = kent_init(&_sched_current_task->kent);
	if (i == 0) {
		_sched_current_task->sp = &_sstack;
		_sched_current_task->stack_bottom = &_estack;
		_sched_current_task->pid = 0;
		_sched_current_task->state = TASK_READY;
		_sched_tasktab[0] = _sched_current_task;

		for (i = 1; i < CONFIG_SCHED_MAXTASK; i++)
			_sched_tasktab[i] = NULL;

		i = arch_watchdog_init();

		if (i == 0)
			i = arch_sched_hwtimer_init(CONFIG_SCHED_MAXTASK);
	}

	/*
	 * we don't really need to deallocate resources on error because we
	 * are going to panic anyways if the scheduler fails to initialize.
	 */

	return i;
}

/**
 * Determine whether the specified task is a candidate for execution.
 *
 * @param task: the task
 * @returns whether `task` could be run next
 */
static inline bool sched_task_should_run(const struct task *task)
{
	enum task_state state = task->state;

	if (state == TASK_QUEUE || state == TASK_READY || state == TASK_IOWAIT)
		return true;

	return false;
}

void *sched_process_switch(void *curr_sp)
{
	struct task *tmp;
	pid_t nextpid = _sched_current_task->pid;
	_sched_current_task->sp = curr_sp;

	if (_sched_current_task->state != TASK_SLEEP && _sched_current_task->state != TASK_IOWAIT)
		_sched_current_task->state = TASK_QUEUE;

	while (1) {
		nextpid++;
		nextpid %= CONFIG_SCHED_MAXTASK;

		tmp = _sched_tasktab[nextpid];
		if (tmp != NULL && sched_task_should_run(tmp)) {
			_sched_current_task = tmp;
			break;
		}
		/* TODO: Add idle thread */
	}

	_sched_current_task->state = TASK_READY;
	return _sched_current_task->sp;
}

struct task *sched_fork(struct task *parent)
{
	pid_t pid;
	struct task *child = malloc(sizeof(*child));

	if (child == NULL)
		goto err_alloc;

	for (pid = 0; pid < CONFIG_SCHED_MAXTASK; pid++) {
		if (_sched_tasktab[pid] == NULL)
			break;
	}
	if (pid == CONFIG_SCHED_MAXTASK)
		goto err_maxtask;

	child->kent.parent = &parent->kent;
	child->kent.destroy = task_destroy;
	if (kent_init(&child->kent) != 0)
		goto err_kent;

	child->pid = pid;
	return child;

err_kent:
err_maxtask:
	free(child);
err_alloc:
	return NULL;
}

struct task *sched_current_task(void)
{
	return _sched_current_task;
}

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
