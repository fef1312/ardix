/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

.extern sched_process_switch

/* void handle_pend_sv(void); */
func_begin handle_pend_sv
	/*
	 * Some registers have already been saved by hardware at this point,
	 * we only need to take care of r4-r11 and lr (the latter of which is
	 * required because lr is overwritten when entering the irq).
	 * The stuff we push onto the stack manually looks about like this:
	 *
	 * <<< stack grow direction (decreasing addresses) <<<
	 * r4 r5 r6 r7 r8 r9 r10 r11 lr
	 */

	push	{r4-r11,lr}

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

	pop	{r4-r11,lr}

	clrex

	bx	lr

func_end handle_pend_sv

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
