/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>

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
