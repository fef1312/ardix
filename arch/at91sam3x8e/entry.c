/* See the end of this file for copyright, license, and warranty information. */

#include <arch/hardware.h>

#include <ardix/types.h>
#include <ardix/sched.h>
#include <ardix/syscall.h>

#include <errno.h>
#include <stddef.h>

#include <config.h>

#ifdef CONFIG_CHECK_SYSCALL_SOURCE
/* syscall.S */
extern uint16_t __syscall_return_point;
#endif

void enter_syscall(struct exc_context *context)
{
	enum syscall number = sc_num(context);
	long (*handler)(sysarg_t arg1, sysarg_t arg2, sysarg_t arg3,
			sysarg_t arg4, sysarg_t arg5, sysarg_t arg6);
	long sc_ret;

#	ifdef CONFIG_CHECK_SYSCALL_SOURCE
	/*
	 * We need to ignore the program counter's LSB because the CPU uses
	 * that as a flag for whether it's operating in ARM or Thumb mode;
	 * the instructions are always 2-byte aligned.  Additionally, the PC
	 * points to the instruction *after* the SVC, not SVC itself.
	 */
	if (((uintptr_t)regs->sp->pc & 0xfffffffe) != (uintptr_t)&__syscall_return_point) {
		sc_set_rval(regs, -EACCES);
		return;
	}
#	endif

	if (number > NSYSCALLS) {
		sc_set_rval(context, -ENOSYS);
		return;
	}

	handler = sys_table[number];
	if (handler == NULL) {
		sc_set_rval(context, -ENOSYS);
		return;
	}

	current->tcb.exc_context = context;

	/* TODO: not every syscall uses the max amount of parameters (duh) */
	sc_ret = handler(sc_arg1(context), sc_arg2(context), sc_arg3(context),
			 sc_arg4(context), sc_arg5(context), sc_arg6(context));

	sc_set_rval(context, sc_ret);
}

void enter_sched(struct exc_context *context)
{
	current->tcb.exc_context = context;
	schedule();
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
