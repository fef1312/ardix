/* See the end of this file for copyright, license, and warranty information. */

.syntax unified
.thumb

.text

.extern sched_process_switch

.thumb_func
.global irq_pend_sv
.type irq_pend_sv, %function

/* void irq_pend_sv(void); */
irq_pend_sv:
	/*
	 * There seems to be a limitation in Thumb which prohibits popping to
	 * any register > r7.  Hence, we need to split the PUSHes and POPs into
	 * multiple operations and MOV register values around.
	 */

	/*
	 * Some registers have already been saved by hardware at this point,
	 * we only need to take care of r4-r11 and lr (the latter of which is
	 * required because lr is overwritten when entering the irq).
	 * The stuff we push onto the stack manually looks about like this:
	 *
	 * <<< stack grow direction (decreasing addresses) <<<
	 * r8 r9 r10 r11 lr r4 r5 r6 r7
	 */

	push	{r4-r7}			/* r4-r7 */

	mov	r3,	r8
	mov	r4,	r9
	mov	r5,	r10
	mov	r6,	r11
	mov	r7,	lr
	push	{r3-r7}			/* r8-r11, lr */

	/*
	 * Now that our stack is completely saved, we can proceed to call the
	 * Kernel's scheduler.  This updates `_current_process` to the process
	 * we want to execute next.
	 */
	/* TODO: Implement banked stack pointer */
	mov	r0,	sp
	bl	sched_process_switch	/* sp = sched_process_switch(sp); */
	mov	sp,	r0

	/*
	 * The new stack pointer contains the state of the new process, so we
	 * load it into our registers using the same procedure as above,
	 * just in reverse order.
	 */

	pop	{r3-r7}			/* r8-r11, lr */
	mov	lr,	r7
	mov	r11,	r6
	mov	r10,	r5
	mov	r9,	r4
	mov	r8,	r3

	pop	{r4-r7}			/* r4-r7 */

	/*
	 * ... and now get the fuck outta here, hoping I never need to touch
	 * this again, even though deep inside I know I will return here at some
	 * point because something broke (or I feel the need to micro-optimize).
	 */
	bx	lr

.size irq_pend_sv, .-irq_pend_sv

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
