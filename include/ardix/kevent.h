/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/list.h>

#include <toolchain.h>

/**
 * @defgroup event Event Subsystem
 *
 * Events are a simple message broadcast mechanism to notify the kernel about
 * changes in hardware in an asynchronous way.  Messages are enqueued from irq
 * context and then processed in the scheduling routine before choosing a new
 * task to run.  Using this mechanism, tasks can be suspended to I/O sleep and
 * then woken up again when the underlying device driver has received an irq
 * indicating the device is ready for new data.
 *
 * @{
 */

enum kevent_kind {
	KEVENT_DEVICE,

	KEVENT_KIND_COUNT,
};

/**
 * @brief kevent base structure.
 * Every type of kevent has its own structure embedding this one.
 */
struct kevent {
	struct kent kent;
	struct list_head link;	/**< list node for the event queue */
	enum kevent_kind kind;
};

/** @brief Initialize the kevent subsystem. */
void kevents_init(void);

/** @brief Process all kevents in the queue.  Called by the scheduler. */
void kevents_process(void);

/**
 * @brief Dispatch an event and transfer its ownership to the kevent subsystem.
 * The event's kent must be initialized to have the device driver the event
 * comes from as its parent, and an appropriate destroy callback that is safe
 * to be invoked from non irq context.
 *
 * @param event kevent to dispatch
 */
void kevent_dispatch(struct kevent *event);

/**
 * @brief Add an event listener to the end of the listener queue.
 * The callback will be invoked for every event that is dispatched and matches
 * the event kind.  If its return value is nonzero, event handler processing
 * stops after that callback.  This is useful if you know for sure that you are
 * the only one interested in the event, for example when waiting for I/O.
 *
 * @param kind Kind of kevent to listen for
 * @param cb Callback that will be invoked for every kevent that is dispatched
 * @param extra An optional extra pointer that will be passed to the callback
 * @returns 0 on success, or a negtive error number on failure
 */
int kevent_add_listener(enum kevent_kind kind,
			int (*cb)(struct kevent *event, void *extra),
			void *extra);

/**
 * @brief Remove an event listener.
 *
 * @param kind Kind that was passed to `kevent_add_listener()`
 * @param cb Callback that was passed to `kevent_add_listener()`
 */
void kevent_remove_listener(enum kevent_kind kind,
			    int (*cb)(struct kevent *event, void *extra));

__always_inline void kevent_get(struct kevent *event)
{
	kent_get(&event->kent);
}

__always_inline void kevent_put(struct kevent *event)
{
	kent_put(&event->kent);
}

/** @} */

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
