/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/syscall.h>

#include <unistd.h>

ssize_t read(int fildes, void *buf, size_t nbyte)
{
	return syscall(SYS_read, (sysarg_t)fildes, (sysarg_t)buf, (sysarg_t)nbyte);
}

ssize_t write(int fildes, const void *buf, size_t nbyte)
{
	return syscall(SYS_write, (sysarg_t)fildes, (sysarg_t)buf, (sysarg_t)nbyte);
}

ssize_t sleep(unsigned long int millis)
{
	return syscall(SYS_sleep, (sysarg_t)millis);
}

pid_t exec(int (*entry)(void))
{
	return (pid_t)syscall(SYS_exec, (sysarg_t)entry);
}

void exit(int status)
{
	syscall(SYS_exit, status);

	/* make gcc happy */
	while (1);
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
