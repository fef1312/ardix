/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.section .text.shared

/* void _mutex_lock(uint8_t *lock); */
func_begin _mutex_lock

	mov		r1,	#1		/* uint8_t newval = 1; */

1:	ldrexb		r2,	[r0]		/* uint8_t tmp = __ldrexb(lock); */

	cmp		r2,	#0
	itt		eq
	strexbeq	r2,	r1,	[r0]	/* tmp = __strexb(newval, lock); */
	cmpeq		r2,	#0

	bne		1b

	dmb
	bx		lr

func_end _mutex_lock

/* int _mutex_trylock(uint8_t *lock); */
func_begin _mutex_trylock

	mov		r1,	#1		/* uint8_t newval = 1; */
	/* move lock to r2 to make room in r0 for the return value */
	mov		r2,	r0

	ldrexb		r0,	[r2]		/* uint8_t tmp = __ldrexb(lock); */

	cmp		r0,	#0
	ittt		eq
	strexbeq	r0,	r1,	[r2]	/* tmp = __strexb(newval, lock); */
	cmpeq		r0,	#0
	dmbeq					/* if (tmp == 0) __dmb(); */

	bx		lr			/* return tmp; */

func_end _mutex_trylock

/* void _mutex_unlock(uint8_t *lock); */
func_begin _mutex_unlock

	mov	r1,	#0
	strb	r1,	[r0]
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
