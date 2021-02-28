/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/syscall.h>

#include <toolchain.h>

#define sys_table_entry(number, func) \
	[number] (int (*)(sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t, sysarg_t))(func)

__rodata
int (*const sys_table[NSYSCALLS])(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
				  sysarg_t arg4, sysarg_t arg5, sysarg_t arg6) = {
	sys_table_entry(SYSCALL_READ, &sys_read),
	sys_table_entry(SYSCALL_WRITE, &sys_write),
};

int sys_stub(void)
{
	return -ENOSYS;
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
