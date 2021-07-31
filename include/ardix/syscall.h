/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch-generic/syscall.h>

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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
