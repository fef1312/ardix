/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/sched.h>
#include <arch/hardware.h>
#include <arch/interrupt.h>

#include <ardix/atomic.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>

#include <errno.h>
#include <string.h>

void handle_sys_tick(void)
{
	/*
	 * fire a PendSV exception and do the actual context switching there
	 * because the docs say you're supposed to do it that way
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

void arch_task_init(struct task *task, void (*entry)(void))
{
	struct reg_snapshot *regs = task->stack_bottom - sizeof(*regs);
	task->sp = regs;

	memset(regs, 0, sizeof(*regs));
	regs->hw.pc = entry;
	regs->hw.psr = 0x01000000;
	regs->sw.lr = (void *)0xfffffff9;
}

void yield(enum task_state state)
{
	REG_SYSTICK_VAL = 0U; /* Reset timer (TODO: don't do this lmao) */
	current->state = state;
	arch_irq_invoke(IRQNO_PEND_SV);
}

__naked __noreturn static void idle_task_entry(void)
{
	__asm__ volatile(
"1:	b	1b	\n"
	:::
	);
}

int arch_idle_task_init(struct task *task)
{
	void *sp = malloc(sizeof(struct reg_snapshot));
	if (sp == NULL)
		return -ENOMEM;

	task->stack_bottom = sp + sizeof(struct reg_snapshot);
	arch_task_init(task, idle_task_entry);
	task->lastexec = 0;
	task->sleep_usecs = 0;
	task->state = TASK_READY;
	task->pid = -1;
	return 0;
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
