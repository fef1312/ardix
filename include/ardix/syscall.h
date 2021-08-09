/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch-generic/syscall.h>

#include <ardix/types.h>

#include <errno.h>
#include <toolchain.h>

enum syscall {
	SYS_read		= ARCH_SYS_read,
	SYS_write		= ARCH_SYS_write,
	SYS_sleep		= ARCH_SYS_sleep,
	NSYSCALLS
};

/** Perform an indirect system call. */
long syscall(enum syscall number, ...);

/** The table of system call handlers, indexed by syscall number. */
extern long (*const sys_table[NSYSCALLS])(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
					  sysarg_t arg4, sysarg_t arg5, sysarg_t arg6);

/* catchall handler that returns -ENOSYS */
long sys_stub(void);

long sys_read(int fd, void *buf, size_t len);
long sys_write(int fd, const void *buf, size_t len);
long sys_sleep(unsigned long millis);

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
