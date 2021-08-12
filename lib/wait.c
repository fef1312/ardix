/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/syscall.h>

#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *stat_loc, int options)
{
	return (pid_t)syscall(SYS_waitpid, (sysarg_t)pid, (sysarg_t)stat_loc, (sysarg_t)options);
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
