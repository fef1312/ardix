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
	SYS_malloc		= ARCH_SYS_malloc,
	SYS_free		= ARCH_SYS_free,
	SYS_exec		= ARCH_SYS_exec,
	SYS_exit		= ARCH_SYS_exit,
	SYS_waitpid		= ARCH_SYS_waitpid,
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
long sys_malloc(size_t size);
void sys_free(void *ptr);
long sys_exec(int (*entry)(void));
void sys_exit(int code);
long sys_waitpid(pid_t pid, int *stat_loc, int options);

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
