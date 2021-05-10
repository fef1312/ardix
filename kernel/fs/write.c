/* See the end of this file for copyright, license, and warranty information. */

#include <arch/serial.h>

#include <ardix/dma.h>
#include <ardix/malloc.h>
#include <ardix/serial.h>
#include <ardix/syscall.h>
#include <ardix/userspace.h>

#include <errno.h>
#include <stddef.h>
#include <toolchain.h>

ssize_t sys_write(int fd, __user const void *buf, size_t len)
{
	ssize_t ret;
	struct dmabuf *dma;

	if (fd != 1) /* we only support stdout (serial console) right now */
		return -EBADF;

	dma = dmabuf_create(&serial_default_device->device, len);
	if (dma == NULL)
		return -ENOMEM;

	copy_from_user(dma->data, buf, len);
	/* TODO: reschedule if blocking */
	ret = serial_write_dma(serial_default_device, dma);
	dmabuf_put(dma);

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
