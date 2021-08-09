/* See the end of this file for copyright, license, and warranty information. */

.syntax unified
.thumb
.cpu cortex-m3

.macro func_begin name
	.global \name
	.type \name, "function"
\name :
.endm

.macro func_end name
	.size \name, .-\name
.endm

.macro prepare_entry
	/*
	 * Hardware automatically saves r0-r3, r12, pc, lr, and psr onto the
	 * stack that was used before exception entry.  Bit 2 in lr is 1 if the
	 * psp was used, and 0 if msp was used.  We store the appropriate stack
	 * pointer into r12 and push it onto the stack along with the remaining
	 * registers.
	 */
	tst	lr,	#(1 << 2)
	ite	ne
	mrsne	r12,	psp
	moveq	r12,	sp

	/*
	 * The kernel doesn't like to be interrupted.
	 * We don't wanna be impolite and hurt its feelings.
	 */
	cpsid	i

	/*
	 * This push matches struct exc_context, r12 is the old stack pointer
	 * (which conveniently points to the remaining registers that were
	 * saved by hardware, including the original value of r12 before entry).
	 */
	push	{r4-r12,lr}
.endm

.macro prepare_leave
	pop	{r4-r12,lr}

	clrex

	/* Turn interrupts back on */
	cpsie	i

	tst	lr,	#(1 << 2)
	ite	ne
	msrne	psp,	r12
	moveq	sp,	r12

.endm prepare_leave

/*
 * This file is part of Ardix.
 * Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
