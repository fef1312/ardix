/* See the end of this file for copyright, license, and warranty information. */

#include <arch-generic/do_switch.h>
#include <arch-generic/sched.h>
#include <arch/hardware.h>
#include <arch/interrupt.h>
#include <arch/linker.h>

#include <ardix/atomic.h>
#include <ardix/kevent.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/serial.h>

#include <errno.h>
#include <string.h>

volatile unsigned long int tick = 0;

static unsigned int systick_reload;
static unsigned int tick_freq;

void handle_sys_tick(void)
{
	tick++;

	/*
	 * fire a PendSV exception and do the actual context switching there
	 * because the docs say you're supposed to do it that way
	 */
	if (!is_atomic())
		SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

int arch_sched_init(unsigned int freq)
{
	tick_freq = freq;
	systick_reload = SystemCoreClock / freq;
	if ((systick_reload & SysTick_LOAD_RELOAD_Msk) != systick_reload)
		return 1;

	/* no subgrouping */
	NVIC_SetPriorityGrouping(0b011);

	SysTick_Config(systick_reload);

	/* give PendSV the lowest priority so it doesn't interrupt irq handlers */
	__set_BASEPRI(0);
	NVIC_SetPriority(PendSV_IRQn, 0xf);

	return 0;
}

void arch_task_init(struct task *task, void (*entry)(void))
{
	/* TODO: Use separate stacks for kernel and program */
	struct hw_context *hw_context = task->bottom - sizeof(*hw_context);
	struct exc_context *exc_context = (void *)hw_context - sizeof(*exc_context);

	memset(hw_context, 0, task->bottom - (void *)hw_context);
	hw_context->pc = entry;
	hw_context->psr = 0x01000000; /* Thumb = 1, unprivileged */

	exc_context->sp = hw_context;
	exc_context->lr = (void *)0xfffffff9; /* leave exception, use MSP */

	memset(&task->tcb, 0, sizeof(task->tcb));
	task->tcb.context.sp = exc_context;
	task->tcb.context.pc = _leave;
}

extern void _idle(void);

int arch_idle_task_init(struct task *task)
{
	void *stack = malloc(CONFIG_STACK_SIZE);
	if (stack == NULL)
		return -ENOMEM;

	task->bottom = stack + CONFIG_STACK_SIZE; /* full-descending stack */
	arch_task_init(task, _idle);
	task->sleep = 0;
	task->last_tick = 0;
	task->state = TASK_READY;
	task->pid = -1;
	return 0;
}

unsigned long int ms_to_ticks(unsigned long int ms)
{
	return ( ms * (unsigned long int)tick_freq ) / 1000lu /* 1 s = 1000 ms */;
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
