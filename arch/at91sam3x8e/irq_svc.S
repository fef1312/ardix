/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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
