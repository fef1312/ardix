/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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

	pop	{r3}
	mov	r12,	r3

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
