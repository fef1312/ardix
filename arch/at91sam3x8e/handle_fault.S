/* See the end of this file for copyright, license, and warranty information. */

/*
 * See arch_handle_fault()
 */

.include "asm.S"

.text

/* __naked __noreturn void arch_handle_fault(struct reg_snapshot *regs, int irqnum); */
.extern arch_handle_fault

func_begin handle_hard_fault
	push	{r4-r11,lr}
	mov	r0,	sp
	mov	r1,	#0xfffffff3	/* -13 */
	b	arch_handle_fault
func_end handle_hard_fault

func_begin handle_mm_fault
	push	{r4-r11,lr}
	mov	r0,	sp
	mov	r1,	#0xfffffff4	/* -12 */
	b	arch_handle_fault
func_end handle_mm_fault

func_begin handle_bus_fault
	push	{r4-r11,lr}
	mov	r0,	sp
	mov	r1,	#0xfffffff5	/* -11 */
	b	arch_handle_fault
func_end handle_bus_fault

func_begin handle_usage_fault
	push	{r4-r11,lr}
	mov	r0,	sp
	mov	r1,	#0xfffffff6	/* -10 */
	b	arch_handle_fault
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
