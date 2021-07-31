/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

/* this is only invoked from user space, obviously */
.section .text.shared

/* int syscall(int number, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6); */
func_begin syscall

	/*
	 *   arg  | syscall |  AAPCS
	 * -------|---------|---------
	 * number | r7      | r0
	 * arg1   | r0      | r1
	 * arg2   | r1      | r2
	 * arg3   | r2      | r3
	 * arg4   | r3      | sp + 0
	 * arg5   | r4      | sp + 4
	 * arg6   | r5      | sp + 8
	 */

	push	{r4-r5,r7}

	mov	r7,	r0		/* number */
	mov	r0,	r1		/* arg1 */
	mov	r1,	r2		/* arg2 */
	mov	r2,	r3		/* arg3 */

	/* stack params begin at 12 bytes offset because we already pushed three registers */
	ldr	r3,	[sp, #12]	/* arg4 */
	ldr	r4,	[sp, #16]	/* arg5 */
	ldr	r5,	[sp, #20]	/* arg6 */

	svc	#0

	pop	{r4-r5,r7}

	/* r0 (return value) is set by the kernel */
	bx	lr

func_end syscall

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
