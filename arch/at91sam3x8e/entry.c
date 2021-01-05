/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <arch/entry.h>
#include <arch/hardware.h>

#include <ardix/syscall.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>
#include <unistd.h>

void arch_enter(void *sp)
{
	struct reg_snapshot *regs = sp;
	enum syscall sc_num = arch_syscall_num(regs);
	int (*handler)(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
		       sysarg_t arg4, sysarg_t arg5, sysarg_t arg6);
	int sc_ret;

	if (sc_num > NSYSCALLS) {
		arch_syscall_set_rval(regs, -EINVAL);
		return;
	}

	handler = sys_table[sc_num];
	if (handler == NULL) {
		arch_syscall_set_rval(regs, -EINVAL);
		return;
	}

	/* TODO: not every syscall uses the max amount of parameters (duh) */
	sc_ret = handler(arch_syscall_arg1(regs), arch_syscall_arg2(regs), arch_syscall_arg3(regs),
			 arch_syscall_arg4(regs), arch_syscall_arg5(regs), arch_syscall_arg6(regs));

	arch_syscall_set_rval(regs, sc_ret);
}

/*
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
