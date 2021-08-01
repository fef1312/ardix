/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/file.h>
#include <ardix/malloc.h>
#include <ardix/syscall.h>
#include <ardix/userspace.h>

#include <errno.h>
#include <stddef.h>
#include <toolchain.h>

ssize_t sys_write(int fd, __user const void *buf, size_t len)
{
	ssize_t ret;
	void *copy;

	struct file *f = file_get(fd);
	if (f == NULL)
		return -EBADF;

	copy = malloc(len);
	if (copy == NULL) {
		file_put(f);
		return -ENOMEM;
	}

	len = copy_from_user(copy, buf, len);
	ret = file_write(f, copy, len);

	free(copy);
	file_put(f);
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
