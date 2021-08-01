/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/entry.h>
#include <arch/hardware.h>

#include <ardix/syscall.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>

#include <config.h>

#ifdef CONFIG_CHECK_SYSCALL_SOURCE
/* syscall.S */
extern uintptr_t __syscall_entry_point;
#endif

void arch_enter(void *sp)
{
	struct reg_snapshot *regs = sp;
	enum syscall sc_num = arch_syscall_num(regs);
	int (*handler)(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
		       sysarg_t arg4, sysarg_t arg5, sysarg_t arg6);
	int sc_ret;

#	ifdef CONFIG_CHECK_SYSCALL_SOURCE
	/*
	 * We need to ignore the program counter's LSB because the CPU uses
	 * that as a flag for whether it's operating in ARM or Thumb mode
	 * (1 for Thumb); the instructions are always 2-byte aligned.
	 */
	if ((regs->hw.pc & 0xfffffffe) != __syscall_entry_point) {
		arch_syscall_set_rval(regs, -EACCES);
		return;
	}
#	endif

	if (sc_num > NSYSCALLS) {
		arch_syscall_set_rval(regs, -ENOSYS);
		return;
	}

	handler = sys_table[sc_num];
	if (handler == NULL) {
		arch_syscall_set_rval(regs, -ENOSYS);
		return;
	}

	/* TODO: not every syscall uses the max amount of parameters (duh) */
	sc_ret = handler(arch_syscall_arg1(regs), arch_syscall_arg2(regs), arch_syscall_arg3(regs),
			 arch_syscall_arg4(regs), arch_syscall_arg5(regs), arch_syscall_arg6(regs));

	arch_syscall_set_rval(regs, sc_ret);
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
