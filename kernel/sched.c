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

static struct task *tasktab[CONFIG_SCHED_MAXTASK];
struct task *current;

static struct task idle_task;

static void task_destroy(struct kent *kent)
{
	struct task *task = container_of(kent, struct task, kent);
	tasktab[task->pid] = NULL;
	free(task);
}

int sched_init(void)
{
	int i;

	current = malloc(sizeof(*current));
	if (current == NULL)
		return -ENOMEM;

	current->kent.parent = kent_root;
	current->kent.destroy = task_destroy;
	i = kent_init(&current->kent);
	if (i != 0)
		goto out;

	current->sp = &_sstack;
	current->stack_bottom = &_estack;
	current->pid = 0;
	current->state = TASK_READY;
	tasktab[0] = current;

	for (i = 1; i < CONFIG_SCHED_MAXTASK; i++)
		tasktab[i] = NULL;

	i = arch_watchdog_init();
	if (i != 0)
		goto out;

	i = arch_sched_hwtimer_init(CONFIG_SCHED_FREQ);
	if (i != 0)
		goto out;

	i = arch_idle_task_init(&idle_task);
	if (i != 0)
		goto out;

	/*
	 * we don't really need to deallocate resources on error because we
	 * are going to panic anyways if the scheduler fails to initialize.
	 */
out:
	return i;
}

/**
 * Determine whether the specified task is a candidate for execution.
 *
 * @param task: the task
 * @returns whether `task` could be run next
 */
static inline bool can_run(const struct task *task)
{
	enum task_state state = task->state;
	return state == TASK_QUEUE || state == TASK_READY;
}

void *sched_process_switch(void *curr_sp)
{
	struct task *tmp;
	int i;
	pid_t nextpid = current->pid;
	current->sp = curr_sp;

	if (current->state != TASK_SLEEP && current->state != TASK_IOWAIT)
		current->state = TASK_QUEUE;

	for (i = 0; i < CONFIG_SCHED_MAXTASK; i++) {
		nextpid++;
		nextpid %= CONFIG_SCHED_MAXTASK;

		tmp = tasktab[nextpid];
		if (tmp != NULL && can_run(tmp)) {
			current = tmp;
			break;
		}
	}

	if (i == CONFIG_SCHED_MAXTASK)
		current = &idle_task;

	current->state = TASK_READY;
	return current->sp;
}

struct task *sched_fork(struct task *parent)
{
	pid_t pid;
	struct task *child = malloc(sizeof(*child));

	if (child == NULL)
		goto err_alloc;

	for (pid = 0; pid < CONFIG_SCHED_MAXTASK; pid++) {
		if (tasktab[pid] == NULL)
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
