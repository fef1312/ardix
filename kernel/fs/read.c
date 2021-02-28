/* SPDX-License-Identifier: GPL-3.0-or-later */
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
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
