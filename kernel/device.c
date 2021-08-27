/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/device.h>
#include <ardix/kent.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>

struct kent *devices_kent = NULL;

static void devices_destroy(struct kent *kent)
{
	/* should never be executed because the root devices kent is immortal */
	kfree(kent);
}

/** Initialize the devices subsystem. */
int devices_init(void)
{
	if (devices_kent != NULL)
		return -EEXIST;

	devices_kent = kmalloc(sizeof(*devices_kent), MEM_KERNEL);
	if (devices_kent == NULL)
		return -ENOMEM;

	devices_kent->parent = kent_root;
	devices_kent->destroy = devices_destroy;

	return kent_init(devices_kent);
}

static void device_destroy(struct kent *kent)
{
	struct device *dev = kent_to_device(kent);
	kfree(dev);
}

int device_init(struct device *dev)
{
	if (dev->kent.destroy == NULL)
		dev->kent.destroy = device_destroy;
	if (dev->kent.parent == NULL)
		dev->kent.parent = devices_kent;

	mutex_init(&dev->lock);
	return kent_init(&dev->kent);
}

static void device_kevent_destroy(struct kent *kent)
{
	struct kevent *event = container_of(kent, struct kevent, kent);
	struct device_kevent *device_kevent = container_of(event, struct device_kevent, kevent);
	kfree(device_kevent);
}

struct device_kevent *device_kevent_create(struct device *device, enum device_kevent_flags flags)
{
	struct device_kevent *event = kmalloc(sizeof(*event), MEM_KERNEL | MEM_ATOMIC);
	if (event == NULL)
		return NULL;

	event->flags = flags;
	event->kevent.kind = KEVENT_DEVICE;

	event->kevent.kent.parent = &device->kent;
	event->kevent.kent.destroy = device_kevent_destroy;
	int err = kent_init(&event->kevent.kent);
	if (err) {
		kfree(event);
		event = NULL;
	}

	return event;
}

void device_kevent_create_and_dispatch(struct device *device, enum device_kevent_flags flags)
{
	struct device_kevent *event = device_kevent_create(device, flags);
	if (event != NULL)
		kevent_dispatch(&event->kevent);
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
