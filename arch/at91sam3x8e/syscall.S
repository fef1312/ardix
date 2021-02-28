/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

.syntax unified
.thumb

/* this is only invoked from user space, obviously */
.section .text.shared

.thumb_func
.global syscall
.type syscall, %function
/* int syscall(int number, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6); */
syscall:
	push	{r4-r5,r7}

	/*
	 * I HAVE NO IDEA WHY THE  F U C K  GCC THINKS I CAN'T MOV TWO LOW REGS
	 * IN ONE INSTRUCTION WHEN THAT IS  L I T E R A L L Y  IN THE ASM OUTPUT
	 * OF GCC WHEN COMPILING SOME C CODE.  BUT AT THIS POINT, I WON'T EVEN
	 * BOTHER WITH WHATEVER SHITTY COMPILE OPTION OR PREPROCESSOR DIRECTIVE
	 * I HAVE TO TURN ON IN ORDER FOR THIS PIECE OF SHIT THAT DARES CALLING
	 * ITSELF A COMPILER TO EAT MY SHIT CODE AND SHIT OUT THE SHITTY BINARY
	 * I WANT.  FUCK YOU, SYSCALLS WILL JUST TAKE 8 INSTRUCTIONS MORE.
	 */

	mov	r4,	r12
	push	{r4}

	mov	r12,	r0		/* syscall number */
	mov	r7,	r12

	mov	r12,	r1		/* arg1 */
	mov	r0,	r12

	mov	r12,	r2		/* arg2 */
	mov	r1,	r12

	mov	r12,	r3		/* arg3 */
	mov	r2,	r12

	pop	{r4}
	mov	r12,	r4

	/* stack params begin at 12 bytes offset because we already pushed three registers */
	ldr	r3,	[sp, #12]	/* arg4 */
	ldr	r4,	[sp, #16]	/* arg5 */
	ldr	r5,	[sp, #20]	/* arg6 */

	svc	#0

	pop	{r4-r5,r7}

	/* r0 (return value) is set by the kernel */
	bx	lr

.size syscall, .-syscall

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
