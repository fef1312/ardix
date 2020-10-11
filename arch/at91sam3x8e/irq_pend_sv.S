/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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
	 * >>> stack grow direction >>>
	 * r7 r6 r5 r4 lr r11 r10 r9 r8
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
