/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atom.h>

void atom_init(atom_t *atom)
{
	atom->count = 0;
}

int atom_get(atom_t *atom)
{
	int tmp;
	int newval;
	atom_t atom_val;

	__asm__ volatile(
"1:	ldrex	%0,	[%3]		\n"	/* atom_val = *atom */
"	add	%1,	%0,	#1	\n"	/* newval = atom_val.count + 1 */
"	strex	%2,	%1,	[%3]	\n"	/* *atom = newval */
"	teq	%2,	#0		\n"	/* store successful? */
"	bne	1b			\n"	/*  -> goto 1 if not */
"	dmb				"	/* memory barrier */
	: "=&r" (atom_val), "=&r" (newval), "=&r" (tmp)
	: "r" (atom)
	: "cc");

	return newval;
}

int atom_put(atom_t *atom)
{
	int tmp;
	int newval;
	atom_t atom_val;

	__asm__ volatile(
"1:	ldrex	%0,	[%3]		\n"	/* atom_val = *atom */
"	sub	%1,	%0,	#1	\n"	/* newval = atom_val.count - 1 */
"	strex	%2,	%1,	[%3]	\n"	/* *atom = newval */
"	teq	%2,	#0		\n"	/* store successful? */
"	bne	1b			\n"	/*   -> goto 1 if not */
"	dmb				"	/* memory barrier */
	: "=&r" (atom_val), "=&r" (newval), "=&r" (tmp)
	: "r" (atom)
	: "cc");

	return newval;
}

int atom_count(atom_t *atom)
{
	return atom->count;
}

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
