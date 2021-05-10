/* See the end of this file for copyright, license, and warranty information. */

.syntax unified
.thumb

.text

.extern arch_enter

.thumb_func
.global irq_svc
.type irq_svc, %function

/* void irq_svc(void); */
irq_svc:
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
	push	{r4-r7}			/* r4-r7 */

	mov	r3,	r8
	mov	r4,	r9
	mov	r5,	r10
	mov	r6,	r11
	mov	r7,	lr
	push	{r3-r7}			/* r8-r11, lr */

	mov	r0,	sp
	bl	arch_enter		/* arch_enter(sp); */

	pop	{r3-r7}			/* r8-r11, lr */
	mov	lr,	r7
	mov	r11,	r6
	mov	r10,	r5
	mov	r9,	r4
	mov	r8,	r3

	pop	{r4-r7}			/* r4-r7 */

	bx	lr

.size irq_svc, .-irq_svc

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
