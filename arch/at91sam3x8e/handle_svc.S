/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* void arch_enter(struct exc_context *context); */
.extern arch_enter

/* void handle_svc(void); */
func_begin handle_svc

	prepare_entry
	mov	r0,	sp
	bl	arch_enter		/* arch_enter(sp); */
	prepare_leave
	bx	lr

func_end handle_svc

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
