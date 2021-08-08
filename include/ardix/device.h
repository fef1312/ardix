/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/mutex.h>
#include <ardix/types.h>
#include <ardix/util.h>

#include <stddef.h>
#include <toolchain.h>

/** Top-level abstraction for any device connected to the system. */
struct device {
	struct kent kent;
	struct mutex lock;
	ssize_t (*read)(void *dest, struct device *device, size_t size, off_t offset);
	ssize_t (*write)(struct device *device, const void *src, size_t size, off_t offset);
};

/** Cast a kent out to its containing struct device */
#define kent_to_device(ptr) container_of(ptr, struct device, kent)

extern struct kent *devices_kent;

enum device_channel {
	DEVICE_CHANNEL_IN,
	DEVICE_CHANNEL_OUT,
};

struct device_kevent {
	struct kevent kevent;
	enum device_channel channel;
};

__always_inline struct device_kevent *kevent_to_device_kevent(struct kevent *event)
{
	return container_of(event, struct device_kevent, kevent);
}

__always_inline struct device *kevent_to_device(struct kevent *event)
{
	return container_of(event->kent.parent, struct device, kent);
}

/**
 * @brief Create a new kevent for the specified device **without dispatching it**.
 *
 * @param device Device the event refers to
 * @param channel Which channel (in or out) the event applies to
 * @returns The created event, or `NULL` if out of memory
 */
struct device_kevent *device_kevent_create(struct device *device, enum device_channel channel);

/**
 * @brief Convenience wrapper for creating and immediately dispatching a device kevent.
 *
 * @param device Device the event refers to
 * @param channel Which channel (in or out) the event applies to
 */
void device_kevent_create_and_dispatch(struct device *device, enum device_channel channel);

/** Initialize the devices subsystem. */
int devices_init(void);

/**
 * @brief Initialize a device and add it to the device tree.
 *
 * @param dev: device to initialze
 * @returns 0 on success, or a negative error code on failure
 */
int device_init(struct device *dev);

/** @brief Increment a device's reference counter. */
__always_inline void device_get(struct device *dev)
{
	kent_get(&dev->kent);
}

/** @brief Decrement a device's referece counter. */
__always_inline void device_put(struct device *dev)
{
	kent_put(&dev->kent);
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
