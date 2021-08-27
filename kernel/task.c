/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/do_switch.h>

#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/malloc.h>
#include <ardix/mutex.h>
#include <ardix/sched.h>
#include <ardix/syscall.h>
#include <ardix/task.h>
#include <ardix/userspace.h>
#include <ardix/util.h>

#include <errno.h>
#include <toolchain.h>

#include <arch/debug.h>

static void task_kevent_destroy(struct kent *kent)
{
	struct kevent *kevent = container_of(kent, struct kevent, kent);
	struct task_kevent *task_kevent = container_of(kevent, struct task_kevent, kevent);
	kfree(task_kevent);
}

void task_kevent_create_and_dispatch(struct task *task, int status)
{
	struct task_kevent *event = kmalloc(sizeof(*event), MEM_KERNEL);
	if (event == NULL)
		return; /* TODO: we're fucked here */

	event->kevent.kent.parent = &task->kent;
	event->kevent.kent.destroy = task_kevent_destroy;
	event->kevent.kind = KEVENT_TASK;
	kent_init(&event->kevent.kent);
	event->task = task;
	event->status = status;
	kevent_dispatch(&event->kevent);
}

struct dead_child {
	struct list_head link; /* -> task::pending_sigchld */
	struct task *child;
	int status;
};

__noreturn void sys_exit(int status)
{
	struct task *task = current;

	struct task *parent = task_parent(task);
	task_kevent_create_and_dispatch(task, status);

	if (parent->state != TASK_WAITPID) {
		/*
		 * the atomic flag wouldn't actually be needed here, but we use
		 * it anyway because it has a separate heap which is more likely
		 * to have an emergency reserve of memory.  A failing allocation
		 * would *really* be inconvenient here.
		 */
		struct dead_child *entry = kmalloc(sizeof(*entry), MEM_KERNEL | MEM_ATOMIC);
		if (entry == NULL) {
			schedule(); /* TODO: we're severely fucked here */
		}

		entry->child = task;

		mutex_lock(&parent->pending_sigchld_lock);
		list_insert(&parent->pending_sigchld, &entry->link);
		mutex_unlock(&parent->pending_sigchld_lock);
	}

	task->state = TASK_DEAD;

	schedule();

	/* we should never get here, this is only needed to make gcc happy */
	while (1);
}

struct task_kevent_extra {
	struct task *parent;

	/* this is a return value from the listener */
	struct {
		struct task *child;
		int status;
	} ret;
};

static int task_kevent_listener(struct kevent *event, void *_extra)
{
	struct task_kevent_extra *extra = _extra;
	struct task_kevent *task_kevent = container_of(event, struct task_kevent, kevent);
	struct task *child = task_kevent->task;

	if (extra->parent != task_parent(child))
		return KEVENT_CB_NONE;

	extra->parent->state = TASK_QUEUE;

	extra->ret.child = child;
	extra->ret.status = task_kevent->status;

	return KEVENT_CB_STOP | KEVENT_CB_LISTENER_DEL;
}

/* manually poll for dead children if there is no memory for a kevent listener */
static int waitpid_poll(struct task *parent)
{
	/* mutex is already locked here */

	while (list_is_empty(&parent->pending_sigchld)) {
		mutex_unlock(&parent->pending_sigchld_lock);
		/*
		 * TODO: This has to be gotten rid of when the scheduler
		 *       isn't a simple round robin one anymore!
		 */
		yield(TASK_QUEUE);
		mutex_lock(&parent->pending_sigchld_lock);
	}

	mutex_unlock(&parent->pending_sigchld_lock);

	struct dead_child *dead_child = list_first_entry(&parent->pending_sigchld,
							 struct dead_child,
							 link);
	int status = dead_child->status;

	task_put(dead_child->child);
	list_delete(&dead_child->link);
	kfree(dead_child);

	return status;
}

/* use kevent system to wait for dying children */
static int waitpid_yield(struct task *parent)
{
	/* mutex is already locked here */

	struct task_kevent_extra extra = {
		.parent = parent,
	};

	if (kevent_listener_add(KEVENT_TASK, task_kevent_listener, &extra) == NULL)
		return waitpid_poll(parent);

	mutex_unlock(&parent->pending_sigchld_lock);
	yield(TASK_WAITPID);

	/* extra.ret is set by task_kevent_listener */
	task_put(extra.ret.child);
	return extra.ret.status;
}

long sys_waitpid(pid_t pid, int __user *stat_loc, int options)
{
	struct task *parent = current;

	/*
	 * both waitpid_yield and waitpid_poll expect the mutex
	 * to be locked and will unlock it before returning
	 */
	mutex_lock(&parent->pending_sigchld_lock);

	int status;
	if (list_is_empty(&parent->pending_sigchld))
		status = waitpid_yield(parent);
	else
		status = waitpid_poll(parent);

	copy_to_user(stat_loc, &status, sizeof(*stat_loc));
	return 0;
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
