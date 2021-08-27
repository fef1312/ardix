/* See the end of this file for copyright, license, and warranty information. */

/**
 * @file sched.c
 * @brief Simple round-robin scheduler.
 *
 * Tasks are stored in a lookup table, `tasks`, which is indexed by pid.
 * The global `current` variable points to the task that is currently running,
 * which must only be accessed from scheduling context (i.e. from within a
 * syscall or scheduling interrupt handler).
 *
 * When `schedule()` is called, it first processes the kevent queue in which irq
 * handlers store broadcasts for changes in hardware state, such as a DMA buffer
 * having been fully transmitted.  Tasks register an event listener for the
 * event they are waiting for before entering I/O wait, and remove their waiting
 * flag in the listener callback.
 *
 * After all events are processed, `schedule()` iterates over the task table
 * starting from one task after the one that has been currently running, and
 * chooses the first one it encounters that is suitable for being woken back up
 * (i.e. is in state `TASK_QUEUE`).  Thus, the previously running task is only
 * executed again if no other tasks are ready to be executed.  If no task is
 * runnable, the idle task is selected.
 *
 * The last step is performing the in-kernel context switch to the next task
 * to be run, which is done by `do_switch()`.  This routine stores the current
 * register state in the old task's TCB and loads the registers from the new
 * one.  Execution then continues where the task that is switched to previously
 * called `do_switch()`, and eventually returns back to userspace by returning
 * from the exception handler.
 */

#include <arch-generic/do_switch.h>
#include <arch-generic/sched.h>
#include <arch-generic/watchdog.h>

#include <ardix/atomic.h>
#include <ardix/kevent.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/task.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>
#include <string.h>

extern uint32_t _sstack;
extern uint32_t _estack;

static struct task *tasks[CONFIG_SCHED_MAXTASK];
static MUTEX(tasks_lock);
struct task *volatile current;

static struct task kernel_task;
static struct task idle_task;

static void task_destroy(struct kent *kent)
{
	struct task *task = container_of(kent, struct task, kent);

	mutex_lock(&tasks_lock);
	tasks[task->pid] = NULL;
	mutex_unlock(&tasks_lock);

	kfree(task->stack);
	kfree(task);
}

int sched_init(void)
{
	int err;

	kernel_task.kent.parent = kent_root;
	kernel_task.kent.destroy = task_destroy;
	err = kent_init(&kernel_task.kent);
	if (err != 0)
		goto out;

	memset(&kernel_task.tcb, 0, sizeof(kernel_task.tcb));
	kernel_task.bottom = &_estack;
	kernel_task.stack = kernel_task.bottom - CONFIG_STACK_SIZE;
	kernel_task.pid = 0;
	kernel_task.state = TASK_RUNNING;

	list_init(&kernel_task.pending_sigchld);
	mutex_init(&kernel_task.pending_sigchld_lock);

	tasks[0] = &kernel_task;
	current = &kernel_task;

	for (unsigned int i = 1; i < ARRAY_SIZE(tasks); i++)
		tasks[i] = NULL;

	err = arch_watchdog_init();
	if (err != 0)
		goto out;

	idle_task.stack = kmalloc(CONFIG_STACK_SIZE, MEM_USER | MEM_STACK);
	if (idle_task.stack == NULL)
		goto out;
	idle_task.bottom = idle_task.stack + CONFIG_STACK_SIZE;
	idle_task.pid = -1;
	idle_task.state = TASK_QUEUE;
	list_init(&idle_task.pending_sigchld);
	mutex_init(&idle_task.pending_sigchld_lock);
	task_init(&idle_task, _idle);

	err = arch_sched_init(CONFIG_SCHED_FREQ);
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
	case TASK_RUNNING:
		return true;
	case TASK_DEAD:
	case TASK_IOWAIT:
	case TASK_LOCKWAIT:
	case TASK_WAITPID:
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

	if (old->state == TASK_RUNNING)
		old->state = TASK_QUEUE;

	for (unsigned int i = 0; i < ARRAY_SIZE(tasks); i++) {
		/*
		 * increment nextpid before accessing the task table
		 * because it is -1 if the idle task was running
		 */
		nextpid++;
		nextpid %= ARRAY_SIZE(tasks);

		struct task *tmp = tasks[nextpid];
		if (tmp != NULL && can_run(tmp)) {
			new = tmp;
			break;
		}
	}

	if (new == NULL)
		new = &idle_task;

	new->state = TASK_RUNNING;
	new->last_tick = tick;
	current = new;

	atomic_leave();

	if (old != new)
		do_switch(old, new);
}

void yield(enum task_state state)
{
	current->state = state;
	schedule();
}

long sys_sleep(unsigned long int millis)
{
	current->sleep = ms_to_ticks(millis);
	yield(TASK_SLEEP);
	/* TODO: return actual milliseconds */
	/*
	 * TODO: actually, use fucking hardware timers which were specifically
	 *       invented for this exact kind of feature because (1) the tick
	 *       resolution is often less than 1 ms and (2) ticks aren't really
	 *       supposed to be guaranteed to happen at regular intervals and
	 *       (3) the scheduler doesn't even check whether there is a task
	 *       whose sleep period just expired
	 */
	return 0;
}

long sys_exec(int (*entry)(void))
{
	pid_t pid;
	struct task *child = NULL;

	mutex_lock(&tasks_lock);

	for (pid = 1; pid < CONFIG_SCHED_MAXTASK; pid++) {
		if (tasks[pid] == NULL)
			break;
	}
	if (pid == CONFIG_SCHED_MAXTASK) {
		pid = -EAGAIN;
		goto out;
	}

	child = kmalloc(sizeof(*child), MEM_KERNEL);
	if (child == NULL) {
		pid = -ENOMEM;
		goto out;
	}

	child->pid = pid;
	child->stack = kmalloc(CONFIG_STACK_SIZE, MEM_USER | MEM_STACK);
	if (child->stack == NULL) {
		pid = -ENOMEM;
		goto err_stack_malloc;
	}

	child->kent.parent = &current->kent;
	child->kent.destroy = task_destroy;
	kent_init(&child->kent);

	child->bottom = child->stack + CONFIG_STACK_SIZE;
	task_init(child, entry);

	list_init(&child->pending_sigchld);
	mutex_init(&child->pending_sigchld_lock);

	child->state = TASK_QUEUE;
	tasks[pid] = child;
	goto out;

err_stack_malloc:
	kfree(child);
out:
	mutex_unlock(&tasks_lock);
	return pid;
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
