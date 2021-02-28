/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/syscall.h>

#include <ardix/types.h>

#include <errno.h>
#include <toolchain.h>

enum syscall {
	SYSCALL_READ		= ARCH_SYSCALL_READ,
	SYSCALL_WRITE		= ARCH_SYSCALL_WRITE,
	NSYSCALLS
};

/** Perform an indirect system call. */
int syscall(enum syscall number, ...);

/** The table of system call handlers, indexed by syscall number. */
extern int (*const sys_table[NSYSCALLS])(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
					 sysarg_t arg4, sysarg_t arg5, sysarg_t arg6);

/* catchall handler that returns -ENOSYS */
int sys_stub(void);

int sys_read(int fd, void *buf, size_t len);
int sys_write(int fd, const void *buf, size_t len);

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
