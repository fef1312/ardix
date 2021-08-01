/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* int _mutex_lock(int *lock); */
func_begin _mutex_lock

	mov	r1,	#1

1:	ldrex	r2,	[r0]
	cmp	r2,	#0
	itt	eq
	strexeq	r2,	r1,	[r0]
	cmpeq	r2,	#0

	bne	1b

	dmb
	eor	r0,	r0
	bx	lr

func_end _mutex_lock

/* int _mutex_unlock(int *lock); */
func_begin _mutex_unlock

	mov	r1,	#0
	str	r1,	[r0]
	dmb
	bx	lr

func_end _mutex_unlock

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
