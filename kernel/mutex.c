/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/do_switch.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/mutex.h>
#include <ardix/sched.h>
#include <ardix/util.h>

#include <errno.h>
#include <stddef.h>

void mutex_init(struct mutex *mutex)
{
	mutex->lock = 0;
	spin_init(&mutex->wait_queue_lock);
	list_init(&mutex->wait_queue);
}

void mutex_lock(struct mutex *mutex)
{
	if (mutex_trylock(mutex) != 0) {
		struct mutex_wait entry = {
			.task = current,
		};

		spin_lock(&mutex->wait_queue_lock);
		list_insert(&mutex->wait_queue, &entry.link);
		spin_unlock(&mutex->wait_queue_lock);

		yield(TASK_LOCKWAIT);
	}
}

void mutex_unlock(struct mutex *mutex)
{
	struct mutex_wait *waiter = NULL;

	spin_lock(&mutex->wait_queue_lock);
	if (!list_is_empty(&mutex->wait_queue)) {
		waiter = list_first_entry(&mutex->wait_queue, struct mutex_wait, link);
		list_delete(&waiter->link);
	}
	spin_unlock(&mutex->wait_queue_lock);

	if (waiter != NULL) {
		struct task *task = waiter->task;
		current->state = TASK_QUEUE;
		do_switch(current, task);
	} else {
		_mutex_unlock(&mutex->lock);
	}
}

/*
 * This file is part of Ardix.
 * Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
