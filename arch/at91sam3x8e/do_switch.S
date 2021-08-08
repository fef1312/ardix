/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* void _do_switch(struct context *old, struct context *new); */
func_begin _do_switch

	/* ldm/stm can't use sp in the reglist, we need to store it individually */

	stmia	r0!,	{r4-r11}
	str	sp,	[r0]
	str	lr,	[r0, #4]	/* this becomes pc when we are switched back to */

	ldmia	r1!,	{r4-r11}
	ldr	sp,	[r1]
	ldr	pc,	[r1, #4]	/* this is the lr that we stored */

func_end _do_switch

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
