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
	/** @brief Device state has changed */
	KEVENT_DEVICE,
	/** @brief File has changed */
	KEVENT_FILE,

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

/**
 * @brief Flags for kevent callback return values.
 */
enum kevent_cb_flags {
	/** @brief No particular action is taken. */
	KEVENT_CB_NONE			= 0,
	/** @brief Stop processing the other callbacks in the queue after the callback. */
	KEVENT_CB_STOP			= (1 << 0),
	/** @brief Call `kevent_listener_del()` after the callback. */
	KEVENT_CB_LISTENER_DEL		= (1 << 1),
};

/**
 * @brief Identifies a single kevent listener.
 *
 * This is returned by `kevent_listener_add()` and must be passed to
 * `kevent_listener_del()` when the listener is no longer needed.
 * You shouldn't modify the members yourself.
 */
struct kevent_listener {
	struct list_head link;
	int (*cb)(struct kevent *event, void *extra);
	void *extra;
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
 * the event kind.  The return value of this callback is a set of flags, see
 * `enum kevent_cb_flags` for details.
 *
 * @param kind Kind of kevent to listen for
 * @param cb Callback that will be invoked for every kevent that is dispatched
 * @param extra An optional extra pointer that will be passed to the callback
 * @returns The listener (pass to `kevent_listener_del()` when no longer needed)
 */
struct kevent_listener *kevent_listener_add(enum kevent_kind kind,
					    int (*cb)(struct kevent *event, void *extra),
					    void *extra);

/**
 * @brief Remove an event listener.
 *
 * @param listener The listener returned by `kevent_listener_add()`
 */
void kevent_listener_del(struct kevent_listener *listener);

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
