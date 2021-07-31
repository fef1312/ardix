/* See the end of this file for copyright, license, and warranty information. */

#include <arch/hardware.h>
#include <arch/interrupt.h>

void arch_irq_enable(enum irqno irqno)
{
	if (irqno >= 0)
		REG_NVIC_ISER((uint32_t)irqno >> 5) = 1 << ( ((uint32_t)irqno) & 0x1F );
}

void arch_irq_disable(enum irqno irqno)
{
	if (irqno >= 0)
		REG_NVIC_ICER((uint32_t)irqno >> 5) = 1 << ( ((uint32_t)irqno) & 0x1F );
}

void arch_irq_invoke(enum irqno irqno)
{
	if (irqno < 0) {
		switch (irqno) {
		case IRQNO_PEND_SV:
			REG_SCB_ICSR = REG_SCB_ICSR_PENDSVSET_BIT;
			break;

		case IRQNO_SYS_TICK:
			REG_SCB_ICSR = REG_SCB_ICSR_PENDSTSET_BIT;
			break;

		default:
			/* TODO: Implement the rest of interrupts < 0 */
			break;
		}
	} else {
		REG_NVIC_ISPR((uint32_t)irqno >> 5) = 1 << ( ((uint32_t)irqno) & 0x1F );
	}
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
