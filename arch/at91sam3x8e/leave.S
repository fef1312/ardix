/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* void _leave(void); */
func_begin _leave
	pop	{r4-r12,lr}

	cpsie	i

	/* same as for _do_switch: check which stack pointer the task will use */
	tst	lr,	#(1 << 2)
	ite	eq
	moveq	sp,	r12
	msrne	psp,	r12

	bx	lr
func_end _leave

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
