/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <stdarg.h>

#include <ardix/syscall.h>

#include <toolchain.h>
#include <unistd.h>

__shared ssize_t read(int fildes, void *buf, size_t nbyte)
{
	return syscall(SYSCALL_READ, (sysarg_t)fildes, (sysarg_t)buf, (sysarg_t)nbyte);
}

__shared ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	return syscall(SYSCALL_WRITE, (sysarg_t)fildes, (sysarg_t)buf, (sysarg_t)nbyte);
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
