/* See the end of this file for copyright, license, and warranty information. */

/**
 * @file kevent implementation.
 *
 * This implementation is kinda fucked, because it basically gambles on system
 * ticks
 */

#include <ardix/atom.h>
#include <ardix/malloc.h>
#include <ardix/mutex.h>
#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/list.h>

#include <errno.h>
#include <stddef.h>

/* event listeners indexed by event type */
static struct list_head kev_listeners[KEVENT_KIND_COUNT];
static MUTEX(kev_listeners_lock);

struct kevent_queue {
	struct list_head list;	/* -> kevent::link */
	struct mutex lock;
};

/* event queues indexed by event type */
static struct kevent_queue kev_queues[KEVENT_KIND_COUNT];

/**
 * irqs cannot sleep or block, so we can only *try* to claim the lock on
 * an event queue when dispatching an event.  If this fails (which should
 * happen only rarely), we store the event in this (unsorted) pile and sort them
 * out before processing the event queue.
 */
LIST_HEAD(kev_cache);
MUTEX(kev_cache_lock);

void kevents_init(void)
{
	for (int i = 0; i < KEVENT_KIND_COUNT; i++) {
		list_init(&kev_listeners[i]);
		list_init(&kev_queues[i].list);
		mutex_init(&kev_queues[i].lock);
	}
}

/* called from scheduler context only */
static inline void process_single_queue(struct kevent_queue *queue, struct list_head *listeners)
{
	struct kevent *event, *tmp_event;

	/*
	 * This method runs from scheduler context which has lower exception
	 * priority than irqs, so in theory this should never fail.  Still, we
	 * only use trylock just in case.
	 */
	if (mutex_trylock(&queue->lock) == 0) {
		list_for_each_entry_safe(&queue->list, event, tmp_event, link) {
			struct kevent_listener *listener, *tmp_listener;

			list_for_each_entry_safe(listeners, listener, tmp_listener, link) {
				int cb_ret = listener->cb(event, listener->extra);

				if (cb_ret & KEVENT_CB_LISTENER_DEL) {
					list_delete(&listener->link);
					kfree(listener);
				}

				if (cb_ret & KEVENT_CB_STOP)
					break;
			}

			list_delete(&event->link);
			kevent_put(event);
		}

		mutex_unlock(&queue->lock);
	}
}

/* called from scheduler context only */
void kevents_process(void)
{
	/*
	 * Same thing as for process_single_queue: This should never fail
	 * because scheduling interrupts have the lowest exception priority.
	 */
	if (mutex_trylock(&kev_cache_lock) == 0) {
		struct kevent *cursor, *tmp;
		list_for_each_entry_safe(&kev_cache, cursor, tmp, link) {
			list_delete(&cursor->link);
			list_insert(&kev_queues[cursor->kind].list, &cursor->link);
		}

		mutex_unlock(&kev_cache_lock);
	}

	for (int i = 0; i < KEVENT_KIND_COUNT; i++)
		process_single_queue(&kev_queues[i], &kev_listeners[i]);
}

void kevent_dispatch(struct kevent *event)
{
	struct kevent_queue *queue = &kev_queues[event->kind];

	if (mutex_trylock(&queue->lock) == 0) {
		list_insert(&queue->list, &event->link);
		mutex_unlock(&queue->lock);
	} else {
		/*
		 * If we got to here it means we preempted the scheduler.
		 * We just toss the event into a temporary pile and let the
		 * scheduler sort out the mess when it calls kevents_process()
		 * the next time.
		 */
		if (mutex_trylock(&kev_cache_lock) == 0) {
			list_insert(&kev_cache, &event->link);
			mutex_unlock(&kev_cache_lock);
		} else {
			/*
			 * If we ever make it to here, something of unfathomable stupidity has
			 * happened because there are only two contexts from which we are supposed
			 * to be accessing the event queue--irq and scheduler.  That means we always
			 * have either the main queue or the temporary cache available to us, and
			 * if not, we forgot to release a lock during yet another sleep deprived
			 * episode of late night coding.  Time to make us pay for what we did then.
			 */

		}
	}
}

struct kevent_listener *kevent_listener_add(enum kevent_kind kind,
					    int (*cb)(struct kevent *, void *),
					    void *extra)
{
	struct kevent_listener *listener = kmalloc(sizeof(*listener));

	if (listener != NULL) {
		listener->cb = cb;
		listener->extra = extra;

		mutex_lock(&kev_listeners_lock);
		list_insert(&kev_listeners[kind], &listener->link);
		mutex_unlock(&kev_listeners_lock);
	}

	return listener;
}

void kevent_listener_del(struct kevent_listener *listener)
{
	mutex_lock(&kev_listeners_lock);
	list_delete(&listener->link);
	mutex_unlock(&kev_listeners_lock);

	kfree(listener);
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
