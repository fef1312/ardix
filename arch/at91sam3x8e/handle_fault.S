/* See the end of this file for copyright, license, and warranty information. */

.include "asm.S"

.text

/* __naked __noreturn void handle_fault(struct exc_context *context, int irqnum); */
.extern handle_fault

func_begin handle_hard_fault
	prepare_entry
	mov	r0,	sp
	mov	r1,	#-13		/* IRQNO_HARD_FAULT */
	b	handle_fault
func_end handle_hard_fault

func_begin handle_mm_fault
	prepare_entry
	mov	r0,	sp
	mov	r1,	#-12		/* IRQNO_MM_FAULT */
	b	handle_fault
func_end handle_mm_fault

func_begin handle_bus_fault
	prepare_entry
	mov	r0,	sp
	mov	r1,	#-11		/* IRQNO_BUS_FAULT */
	b	handle_fault
func_end handle_bus_fault

func_begin handle_usage_fault
	prepare_entry
	mov	r0,	sp
	mov	r1,	#-10		/* IRQNO_USAGE_FAULT */
	b	handle_fault
func_end handle_usage_fault

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
