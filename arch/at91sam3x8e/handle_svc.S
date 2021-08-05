/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* int arch_enter(void *sp); */
.extern arch_enter

/* void *sched_switch(void *sp); */
.extern sched_switch

/* void handle_svc(void); */
func_begin handle_svc
	/*
	 * Syscalls on Cortex-M use the following parameter calling convention:
	 *
	 * number: r7
	 * rval: r0
	 * arg1: r0
	 * arg2: r1
	 * arg3: r2
	 * arg4: r3
	 * arg5: r4
	 * arg6: r5
	 */

	/*
	 * like in irq_pend_sv, we save everything on the stack to make early
	 * process switching possible in case the syscall is blocking.
	 */
	push	{r4-r11,lr}

	mov	r0,	sp
	bl	arch_enter		/* int need_resched = arch_enter(sp); */

	cmp	r0,	#0
	beq	svc_out

	mov	r0,	sp
	bl	sched_switch		/* sp = sched_switch(sp); */
	mov	sp,	r0

	clrex

svc_out:
	pop	{r4-r11,lr}

	bx	lr

func_end handle_svc

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
