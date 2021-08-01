/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/sched.h>
#include <arch/hardware.h>
#include <arch/interrupt.h>

#include <ardix/atomic.h>
#include <ardix/sched.h>

#include <string.h>

void irq_sys_tick(void)
{
	/*
	 * fire a PendSV interrupt and do the actual context switching there
	 * because it is faster that way (according to the docs, at least)
	 */
	if (!is_atomic_context())
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

void arch_sched_task_init(struct task *task, void (*entry)(void))
{
	struct reg_snapshot *regs = task->stack_bottom - sizeof(*regs);
	task->sp = regs;

	memset(regs, 0, sizeof(*regs));
	regs->hw.pc = (uintptr_t)entry;
	regs->hw.psr = 0x01000000U;
	regs->sw.lr = 0xfffffff9U;
}

void yield(enum task_state state)
{
	REG_SYSTICK_VAL = 0U; /* Reset timer */
	current->state = state;
	arch_irq_invoke(IRQNO_PEND_SV);
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
