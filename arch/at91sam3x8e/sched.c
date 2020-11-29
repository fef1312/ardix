/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <arch/sched.h>
#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>

#include <ardix/string.h>
#include <ardix/sched.h>

#include <stdbool.h>
#include <toolchain.h>

void irq_sys_tick(void)
{
	/*
	 * fire a PendSV interrupt and do the actual context switching there
	 * because it is faster that way (according to the docs, at least)
	 */
	arch_irq_invoke(IRQNO_PEND_SV);
}

/**
 * Set the NVIC priority grouping field in `AIRCR`.
 * Only values from 0..7 are allowed, see the SAM3X documentation.
 *
 * @param prio_group: The new priority grouping value.
 */
static inline void sched_nvic_set_prio_group(uint32_t prio_group)
{
	uint32_t reg_val = REG_SCB_AIRCR;

	reg_val &= ~(REG_SCB_AIRCR_VECTKEY_MASK | REG_SCB_AIRCR_PRIGROUP_MASK);
	reg_val = reg_val
		| REG_SCB_AIRCR_VECTKEY_VAL(REG_SCB_AIRCR_VECTKEY_MAGIC)
		| REG_SCB_AIRCR_PRIGROUP_VAL(prio_group);

	REG_SCB_AIRCR = reg_val;
}

int arch_sched_hwtimer_init(unsigned int freq)
{
	uint32_t ticks = sys_core_clock / freq;
	if (ticks > REG_SYSTICK_LOAD_RELOAD_MASK)
		return 1;

	/* Ensure SysTick and PendSV are preemptive */
	sched_nvic_set_prio_group(0b011);

	REG_SYSTICK_LOAD = (ticks & REG_SYSTICK_LOAD_RELOAD_MASK) - 1;
	REG_SYSTICK_VAL = 0U;
	REG_SYSTICK_CTRL = REG_SYSTICK_CTRL_CLKSOURCE_BIT /* MCK */
			 | REG_SYSTICK_CTRL_TICKINT_BIT /* trigger exception */
			 | REG_SYSTICK_CTRL_ENABLE_BIT; /* enable SysTick */

	return 0;
}

void arch_sched_process_init(struct process *process, void (*entry)(void))
{
	struct reg_snapshot *regs = process->stack_bottom - sizeof(*regs);
	process->sp = regs;

	memset(regs, 0, sizeof(*regs));
	regs->hw.pc = entry;
}

void sched_switch_early(enum proc_state state)
{
	REG_SYSTICK_VAL = 0U; /* Reset timer */
	_current_process->state = state;
	arch_irq_invoke(IRQNO_PEND_SV);
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
