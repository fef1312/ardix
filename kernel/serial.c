/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/device.h>
#include <ardix/ringbuf.h>
#include <ardix/sched.h>
#include <ardix/serial.h>

#include <arch/serial.h>

#include <stddef.h>

int serial_init(struct serial_device *dev, long int baud)
{
	int err = -1;

	if (dev->id < 0)
		return -1; /* invalid dev */

	err = device_init(&dev->device);

	dev->baud = baud;

	dev->rx = ringbuf_create(SERIAL_BUFSZ);
	if (dev->rx == NULL)
		return -1;

	err = arch_serial_init(dev);
	if (err)
		ringbuf_destroy(dev->rx);

	return err;
}

void serial_exit(struct serial_device *dev)
{
	arch_serial_exit(dev);
	ringbuf_destroy(dev->rx);
	dev->id = -1;
}

ssize_t serial_read(void *dest, struct serial_device *dev, size_t len)
{
	ssize_t ret;

	atomic_enter();
	ret = (ssize_t)ringbuf_read(dest, dev->rx, len);
	atomic_leave();

	return ret;
}

ssize_t serial_write(struct serial_device *dev, const void *data, size_t len)
{
	ssize_t ret;

	atomic_enter();
	ret = arch_serial_write(dev, data, len);
	atomic_leave();

	return ret;
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
