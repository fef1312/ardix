/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/syscall.h>

#include <toolchain.h>

#define sys_table_entry(number, func) \
	[number] (long (*)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t))(func)

__rodata
long (*const sys_table[NSYSCALLS])(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
				   sysarg_t arg4, sysarg_t arg5, sysarg_t arg6) = {
	sys_table_entry(SYS_read,		sys_read),
	sys_table_entry(SYS_write,		sys_write),
	sys_table_entry(SYS_sleep,		sys_sleep),
};

long sys_stub(void)
{
	return -ENOSYS;
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
