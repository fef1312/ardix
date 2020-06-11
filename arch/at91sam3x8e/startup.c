/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

#include <stddef.h>
#include <stdint.h>
#include <ardix/string.h>
#include <arch/at91sam3x8e/interrupt.h>

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

/* implementation in init/main.c */
void do_bootstrap(void);

void __isr_reset(void)
{
	/* copy .data to sram */
	memmove(
		&_etext,
		&_srelocate,
		(size_t)&_erelocate - (size_t)&_srelocate
	);
	/* clear .bss */
	memset(&_szero, 0, (size_t)&_ezero - (size_t)&_szero);

	/* start the Kernel */
	do_bootstrap();

	/* halt (this should never be reached) */
	while (1);
}

/**
 * Default ISR for unimplemented interrupts.
 * This will halt the system.
 */
void __isr_stub(void)
{
	while (1);
}

__attribute__((weak, alias("__isr_stub"))) void __isr_nmi(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_hard_fault(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_mem_fault(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_bus_fault(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_usage_fault(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_svc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_debug_mon(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_pend_sv(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_sys_tick(void);

__attribute__((weak, alias("__isr_stub"))) void __isr_supc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_rstc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_rtc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_rtt(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_wdt(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_pmc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_efc0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_efc1(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_uart(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_smc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_pioa(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_piob(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_pioc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_piod(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_usart0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_usart1(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_usart2(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_usart3(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_hsmci(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_twi0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_twi1(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_spi0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_ssc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc1(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc2(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc3(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc4(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc5(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc6(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc7(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_tc8(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_pwm(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_adc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_dacc(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_dmac(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_uotghs(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_trng(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_emac(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_can0(void);
__attribute__((weak, alias("__isr_stub"))) void __isr_can1(void);

__attribute__((section(".vectors")))
const void *exception_table[] = {
	&_estack,		/* initial SP value (stack grows down) */
	&__isr_reset,		/* reset vector */
	NULL,			/* reserved */
	&__isr_hard_fault,	/* hard fault */
	&__isr_mem_fault,	/* hemory management fault */
	&__isr_bus_fault,	/* bus fault */
	&__isr_usage_fault,	/* usage fault */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	&__isr_svc,		/* SVC call (used for syscalls) */
	&__isr_debug_mon,	/* reserved for debug */
	NULL,			/* reserved */
	&__isr_pend_sv,		/* PendSV */
	&__isr_sys_tick,	/* SysTick (used by the scheduler) */

	/*
	 * Ok I am REALLY tired of writing out mnemonics.
	 * Just have a look at include/arch/at91sam3x8e.h for details.
	 */
	&__isr_rstc,
	&__isr_rtc,
	&__isr_rtt,
	&__isr_wdt,
	&__isr_pmc,
	&__isr_efc0,
	&__isr_efc1,
	&__isr_uart,
	&__isr_smc,
	NULL,			/* reserved */
	&__isr_pioa,
	&__isr_piob,
	&__isr_pioc,
	&__isr_piod,
	NULL,			/* reserved */
	NULL,			/* reserved */
	&__isr_usart0,
	&__isr_usart1,
	&__isr_usart2,
	&__isr_usart3,
	&__isr_hsmci,
	&__isr_twi0,
	&__isr_twi1,
	&__isr_spi0,
	NULL,			/* reserved */
	&__isr_ssc,
	&__isr_tc0,
	&__isr_tc1,
	&__isr_tc2,
	&__isr_tc3,
	&__isr_tc4,
	&__isr_tc5,
	&__isr_tc6,
	&__isr_tc7,
	&__isr_tc8,
	&__isr_pwm,
	&__isr_adc,
	&__isr_dacc,
	&__isr_dmac,
	&__isr_uotghs,
	&__isr_trng,
	&__isr_emac,
	&__isr_can0,
	&__isr_can1,
};
