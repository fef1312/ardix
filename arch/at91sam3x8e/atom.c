/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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