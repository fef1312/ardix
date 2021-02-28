/* SPDX-License-Identifier: GPL-3.0-or-later */
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
