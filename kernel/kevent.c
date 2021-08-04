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
	struct list_head list;	/* -> kevent_listener::link */
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
	struct kevent *event, *tmp;

	/*
	 * This method is only invoked from scheduler context which has higher
	 * exception priority than all irqs, so if claiming the lock on this
	 * list fails it means we interrupted the irq.  The current strategy is
	 * to just abort and try again during the next system tick.
	 */
	if (mutex_trylock(&queue->lock) == 0) {
		list_for_each_entry_safe(&queue->list, event, tmp, link) {
			struct kevent_listener *listener;

			list_for_each_entry(listeners, listener, link) {
				int cb_ret = listener->cb(event, listener->extra);

				if (cb_ret & KEVENT_CB_LISTENER_DEL) {
					list_delete(&listener->link);
					free(listener);
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
	 * if this fails it means the scheduling interrupt happened while
	 * processing an irq, just ignore the cache and try again next time
	 * if that is the case
	 */
	if (mutex_trylock(&kev_cache_lock) == 0) {
		struct kevent *cursor, *tmp;
		list_for_each_entry_safe(&kev_cache, cursor, tmp, link) {
			list_insert(&kev_queues[cursor->kind].list, &cursor->link);
			list_delete(&cursor->link);
		}

		mutex_unlock(&kev_cache_lock);
	}

	for (int i = 0; i < KEVENT_KIND_COUNT; i++)
		process_single_queue(&kev_queues[i], &kev_listeners[i]);
}

/* called from irq context only */
void kevent_dispatch(struct kevent *event)
{
	struct kevent_queue *queue = &kev_queues[event->kind];

	if (mutex_trylock(&queue->lock) == 0) {
		list_insert(&queue->list, &event->link);
		mutex_unlock(&queue->lock);
	} else {
		/*
		 * We shouldn't ever be able to get here because irqs don't interrupt
		 * each other and if we get interrupted by the scheduler it doesn't
		 * matter because kevents_process() always releases its lock before
		 * returning again.  If it still happens for whatever stupid reason,
		 * we insert the event in a temporary unsorted cache that is then
		 * ordered by the scheduler.
		 */
		if (mutex_trylock(&kev_cache_lock) == 0) {
			list_insert(&kev_cache, &event->link);
			mutex_unlock(&kev_cache_lock);
		} else {
			/*
			 * If we ever make it to here, something *extremely* stupid
			 * has happened: we couldn't get the lock on the queue (which
			 * shouldn't ever happen in the first place), *and* the cache
			 * is locked as well.  We will have to assume we got suspended
			 * at some point in these if branches, and just try getting
			 * the lock on the original queue again.  If that fails as
			 * well, we just give up and discard the event all together.
			 *
			 * TODO: This solution is of course far from ideal and has to
			 *       be refactored at some point before the first stable
			 *       release.  We'll just deal with that later(TM).
			 */
			if (mutex_trylock(&queue->lock) == 0) {
				list_insert(&queue->list, &event->link);
				mutex_unlock(&queue->lock);
			} else {
				kevent_put(event);
			}
		}
	}
}

struct kevent_listener *kevent_listener_add(enum kevent_kind kind,
					    int (*cb)(struct kevent *, void *),
					    void *extra)
{
	struct kevent_listener *listener = malloc(sizeof(*listener));

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

	free(listener);
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
