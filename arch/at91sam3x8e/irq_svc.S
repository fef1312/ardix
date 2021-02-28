/* SPDX-License-Identifier: GPL-3.0-or-later */
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
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
