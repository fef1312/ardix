/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* int _atom_get(int *count); */
func_begin _atom_get

	ldrex	r1,	[r0]		/* int tmp = atom->count */
	add	r2,	r1,	#1	/* int newval = tmp + 1 */
	strex	r3,	r2,	[r0]	/* atom->count = newval */
	teq	r3,	#0		/* store successful? */
	bne	_atom_get		/*   -> goto _atom_get to try again if not */
	dmb				/* data memory barrier */
	mov	r0,	r2		/* return newval */
	bx	lr

func_end _atom_get

/* int _atom_put(int *count); */
func_begin _atom_put

	ldrex	r1,	[r0]		/* int tmp = atom->count */
	sub	r2,	r1,	#1	/* int newval = tmp - 1 */
	strex	r3,	r2,	[r0]	/* atom->count = newval */
	teq	r3,	#0		/* store successful? */
	bne	_atom_put		/*   -> goto _atom_put to try again if not */
	dmb				/* data memory barrier */
	mov	r0,	r2		/* return newval */
	bx	lr

func_end _atom_put

/*
 * This file is part of Ardix.
 * Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
