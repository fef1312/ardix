/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/do_switch.h>
#include <arch-generic/sched.h>
#include <arch-generic/watchdog.h>

#include <ardix/atomic.h>
#include <ardix/kevent.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>
#include <string.h>

extern uint32_t _sstack;
extern uint32_t _estack;

static struct task *tasktab[CONFIG_SCHED_MAXTASK];
struct task *volatile current;

static struct task idle_task;

static void task_destroy(struct kent *kent)
{
	struct task *task = container_of(kent, struct task, kent);
	tasktab[task->pid] = NULL;
	free(task);
}

int sched_init(void)
{
	int err;

	struct task *ktask = malloc(sizeof(*ktask));
	if (ktask == NULL)
		return -ENOMEM;

	ktask->kent.parent = kent_root;
	ktask->kent.destroy = task_destroy;
	err = kent_init(&ktask->kent);
	if (err != 0)
		goto out;

	memset(&ktask->tcb, 0, sizeof(ktask->tcb));
	ktask->bottom = &_estack;
	ktask->pid = 0;
	ktask->state = TASK_READY;

	tasktab[0] = ktask;
	current = ktask;

	for (unsigned int i = 1; i < ARRAY_SIZE(tasktab); i++)
		tasktab[i] = NULL;

	err = arch_watchdog_init();
	if (err != 0)
		goto out;

	err = arch_sched_init(CONFIG_SCHED_FREQ);
	if (err != 0)
		goto out;

	err = arch_idle_task_init(&idle_task);
	if (err != 0)
		goto out;

	/*
	 * we don't really need to deallocate resources on error because we
	 * are going to panic anyways if the scheduler fails to initialize.
	 */
out:
	return err;
}

/**
 * @brief Determine whether the specified task is a candidate for execution.
 *
 * @param task The task
 * @returns whether `task` could be run next
 */
static inline bool can_run(const struct task *task)
{
	switch (task->state) {
	case TASK_SLEEP:
		return tick - task->last_tick >= task->sleep;
	case TASK_QUEUE:
	case TASK_READY:
		return true;
	case TASK_DEAD:
	case TASK_IOWAIT:
		return false;
	}

	return false; /* this shouldn't be reached */
}

void schedule(void)
{
	atomic_enter();

	struct task *old = current;
	pid_t nextpid = old->pid;

	struct task *new = NULL;

	kevents_process();

	if (old->state == TASK_READY)
		old->state = TASK_QUEUE;
	for (unsigned int i = 0; i < ARRAY_SIZE(tasktab); i++) {
		/*
		 * increment nextpid before accessing the task table
		 * because it is -1 if the idle task was running
		 */
		nextpid++;
		nextpid %= ARRAY_SIZE(tasktab);

		struct task *tmp = tasktab[nextpid];
		if (tmp != NULL && can_run(tmp)) {
			new = tmp;
			break;
		}
	}

	if (new == NULL)
		new = &idle_task;

	new->state = TASK_READY;
	new->last_tick = tick;
	current = new;

	atomic_leave();

	if (old != new)
		do_switch(old, new);
}

void yield(enum task_state state)
{
	struct task *task = current;
	task->state = state;
	schedule();
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

void msleep(unsigned long int ms)
{
	current->sleep = ms_to_ticks(ms);
	yield(TASK_SLEEP);
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
