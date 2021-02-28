/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>

#include <ardix/malloc.h>
#include <ardix/string.h>

#include <stddef.h>
#include <stdint.h>
#include <toolchain.h>

/* from flash.ld */
extern uint32_t _sfixed;	/* fixed area start */
extern uint32_t _efixed;	/* fixed area end */
extern uint32_t _etext;		/* program (.text) end */
extern uint32_t _srelocate;	/* relocate (.data) start */
extern uint32_t _erelocate;	/* relocate end */
extern uint32_t _szero;		/* zero area (.bss) start */
extern uint32_t _ezero;		/* zero area end */
extern uint32_t _sstack;	/* stack start */
extern uint32_t _estack;	/* stack end */
extern uint32_t _sheap;		/* heap start */
extern uint32_t _eheap;		/* heap end */

/* implementation in init/main.c */
void do_bootstrap(void);

__naked void irq_reset(void)
{
	sys_init();

	memmove(&_srelocate, &_etext, (size_t)(&_erelocate) - (size_t)(&_srelocate));
	memset(&_szero, 0, (size_t)(&_ezero) - (size_t)(&_szero));

	/* There is no userspace yet, so the Kernel gets the entire heap for now */
	malloc_init(&_sheap, (size_t)(&_eheap) - (size_t)(&_sheap));

	/* start the Kernel */
	do_bootstrap();

	/* halt (this should never be reached) */
	while (1);
}

/**
 * Default IRQ for unimplemented interrupts.
 * This will halt the system.
 */
void irq_stub(void)
{
	while (1);
}

__weak __alias(irq_stub) void irq_nmi(void);
__weak __alias(irq_stub) void irq_hard_fault(void);
__weak __alias(irq_stub) void irq_mem_fault(void);
__weak __alias(irq_stub) void irq_bus_fault(void);
__weak __alias(irq_stub) void irq_usage_fault(void);
__weak __alias(irq_stub) void irq_svc(void);
__weak __alias(irq_stub) void irq_debug_mon(void);
__weak __alias(irq_stub) void irq_pend_sv(void);
__weak __alias(irq_stub) void irq_sys_tick(void);

__weak __alias(irq_stub) void irq_supc(void);
__weak __alias(irq_stub) void irq_rstc(void);
__weak __alias(irq_stub) void irq_rtc(void);
__weak __alias(irq_stub) void irq_rtt(void);
__weak __alias(irq_stub) void irq_wdt(void);
__weak __alias(irq_stub) void irq_pmc(void);
__weak __alias(irq_stub) void irq_efc0(void);
__weak __alias(irq_stub) void irq_efc1(void);
__weak __alias(irq_stub) void irq_uart(void);
__weak __alias(irq_stub) void irq_smc(void);
__weak __alias(irq_stub) void irq_pioa(void);
__weak __alias(irq_stub) void irq_piob(void);
__weak __alias(irq_stub) void irq_pioc(void);
__weak __alias(irq_stub) void irq_piod(void);
__weak __alias(irq_stub) void irq_usart0(void);
__weak __alias(irq_stub) void irq_usart1(void);
__weak __alias(irq_stub) void irq_usart2(void);
__weak __alias(irq_stub) void irq_usart3(void);
__weak __alias(irq_stub) void irq_hsmci(void);
__weak __alias(irq_stub) void irq_twi0(void);
__weak __alias(irq_stub) void irq_twi1(void);
__weak __alias(irq_stub) void irq_spi0(void);
__weak __alias(irq_stub) void irq_ssc(void);
__weak __alias(irq_stub) void irq_tc0(void);
__weak __alias(irq_stub) void irq_tc1(void);
__weak __alias(irq_stub) void irq_tc2(void);
__weak __alias(irq_stub) void irq_tc3(void);
__weak __alias(irq_stub) void irq_tc4(void);
__weak __alias(irq_stub) void irq_tc5(void);
__weak __alias(irq_stub) void irq_tc6(void);
__weak __alias(irq_stub) void irq_tc7(void);
__weak __alias(irq_stub) void irq_tc8(void);
__weak __alias(irq_stub) void irq_pwm(void);
__weak __alias(irq_stub) void irq_adc(void);
__weak __alias(irq_stub) void irq_dacc(void);
__weak __alias(irq_stub) void irq_dmac(void);
__weak __alias(irq_stub) void irq_uotghs(void);
__weak __alias(irq_stub) void irq_trng(void);
__weak __alias(irq_stub) void irq_emac(void);
__weak __alias(irq_stub) void irq_can0(void);
__weak __alias(irq_stub) void irq_can1(void);

__section(.vectors) const void *exception_table[] = {
	&_estack,		/* initial SP value (stack grows down) */
	&irq_reset,		/* reset vector */
	NULL,			/* reserved */
	&irq_hard_fault,	/* hard fault */
	&irq_mem_fault,		/* memory management fault */
	&irq_bus_fault,		/* bus fault */
	&irq_usage_fault,	/* usage fault */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	&irq_svc,		/* SVC call (used for syscalls) */
	&irq_debug_mon,		/* reserved for debug */
	NULL,			/* reserved */
	&irq_pend_sv,		/* PendSV (used by the scheduler) */
	&irq_sys_tick,		/* SysTick */

	/*
	 * Ok I am REALLY tired of writing out mnemonics.
	 * Just have a look at include/arch/at91sam3x8e.h for details.
	 */
	&irq_supc,
	&irq_rstc,
	&irq_rtc,
	&irq_rtt,
	&irq_wdt,
	&irq_pmc,
	&irq_efc0,
	&irq_efc1,
	&irq_uart,
	&irq_smc,
	NULL,			/* reserved */
	&irq_pioa,
	&irq_piob,
	&irq_pioc,
	&irq_piod,
	NULL,			/* reserved */
	NULL,			/* reserved */
	&irq_usart0,
	&irq_usart1,
	&irq_usart2,
	&irq_usart3,
	&irq_hsmci,
	&irq_twi0,
	&irq_twi1,
	&irq_spi0,
	NULL,			/* reserved */
	&irq_ssc,
	&irq_tc0,
	&irq_tc1,
	&irq_tc2,
	&irq_tc3,
	&irq_tc4,
	&irq_tc5,
	&irq_tc6,
	&irq_tc7,
	&irq_tc8,
	&irq_pwm,
	&irq_adc,
	&irq_dacc,
	&irq_dmac,
	&irq_uotghs,
	&irq_trng,
	&irq_emac,
	&irq_can0,
	&irq_can1,
};

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
