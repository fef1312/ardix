/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/malloc.h>
#include <ardix/serial.h>
#include <ardix/syscall.h>
#include <ardix/userspace.h>

#include <errno.h>
#include <stddef.h>
#include <toolchain.h>

ssize_t sys_read(int fd, __user void *buf, size_t len)
{
	ssize_t ret;
	void *copy;

	if (fd != 0) /* we only support stdin (serial console) right now */
		return -EBADF;

	copy = malloc(len);
	if (copy == NULL)
		return -ENOMEM;

	ret = serial_read(copy, serial_default_device, len);
	if (ret > 0)
		copy_to_user(buf, copy, (size_t)ret);

	free(copy);

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
