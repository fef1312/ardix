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
#include <arch/at91sam3x8e/interrupt.h>
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

/* implementation in init/main.c */
void do_bootstrap(void);

void isr_reset(void)
{
	uint32_t *src;
	uint32_t *dest;

	/* copy .data to sram */
	dest = &_etext;
	src = &_srelocate;
	while (src < &_erelocate)
		*dest++ = *src++;

	/** clear .bss */
	dest = &_szero;
	while (dest <= &_ezero)
		*dest++ = 0;

	/* start the Kernel */
	do_bootstrap();

	/* halt (this should never be reached) */
	while (1);
}

/**
 * Default ISR for unimplemented interrupts.
 * This will halt the system.
 */
void isr_stub(void)
{
	while (1);
}

__weak __alias(isr_stub) void isr_nmi(void);
__weak __alias(isr_stub) void isr_hard_fault(void);
__weak __alias(isr_stub) void isr_mem_fault(void);
__weak __alias(isr_stub) void isr_bus_fault(void);
__weak __alias(isr_stub) void isr_usage_fault(void);
__weak __alias(isr_stub) void isr_svc(void);
__weak __alias(isr_stub) void isr_debug_mon(void);
__weak __alias(isr_stub) void isr_pend_sv(void);
__weak __alias(isr_stub) void isr_sys_tick(void);

__weak __alias(isr_stub) void isr_supc(void);
__weak __alias(isr_stub) void isr_rstc(void);
__weak __alias(isr_stub) void isr_rtc(void);
__weak __alias(isr_stub) void isr_rtt(void);
__weak __alias(isr_stub) void isr_wdt(void);
__weak __alias(isr_stub) void isr_pmc(void);
__weak __alias(isr_stub) void isr_efc0(void);
__weak __alias(isr_stub) void isr_efc1(void);
__weak __alias(isr_stub) void isr_uart(void);
__weak __alias(isr_stub) void isr_smc(void);
__weak __alias(isr_stub) void isr_pioa(void);
__weak __alias(isr_stub) void isr_piob(void);
__weak __alias(isr_stub) void isr_pioc(void);
__weak __alias(isr_stub) void isr_piod(void);
__weak __alias(isr_stub) void isr_usart0(void);
__weak __alias(isr_stub) void isr_usart1(void);
__weak __alias(isr_stub) void isr_usart2(void);
__weak __alias(isr_stub) void isr_usart3(void);
__weak __alias(isr_stub) void isr_hsmci(void);
__weak __alias(isr_stub) void isr_twi0(void);
__weak __alias(isr_stub) void isr_twi1(void);
__weak __alias(isr_stub) void isr_spi0(void);
__weak __alias(isr_stub) void isr_ssc(void);
__weak __alias(isr_stub) void isr_tc0(void);
__weak __alias(isr_stub) void isr_tc1(void);
__weak __alias(isr_stub) void isr_tc2(void);
__weak __alias(isr_stub) void isr_tc3(void);
__weak __alias(isr_stub) void isr_tc4(void);
__weak __alias(isr_stub) void isr_tc5(void);
__weak __alias(isr_stub) void isr_tc6(void);
__weak __alias(isr_stub) void isr_tc7(void);
__weak __alias(isr_stub) void isr_tc8(void);
__weak __alias(isr_stub) void isr_pwm(void);
__weak __alias(isr_stub) void isr_adc(void);
__weak __alias(isr_stub) void isr_dacc(void);
__weak __alias(isr_stub) void isr_dmac(void);
__weak __alias(isr_stub) void isr_uotghs(void);
__weak __alias(isr_stub) void isr_trng(void);
__weak __alias(isr_stub) void isr_emac(void);
__weak __alias(isr_stub) void isr_can0(void);
__weak __alias(isr_stub) void isr_can1(void);

__section(.vectors) const void *exception_table[] = {
	&_estack,		/* initial SP value (stack grows down) */
	&isr_reset,		/* reset vector */
	NULL,			/* reserved */
	&isr_hard_fault,	/* hard fault */
	&isr_mem_fault,		/* hemory management fault */
	&isr_bus_fault,		/* bus fault */
	&isr_usage_fault,	/* usage fault */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	&isr_svc,		/* SVC call (used for syscalls) */
	&isr_debug_mon,		/* reserved for debug */
	NULL,			/* reserved */
	&isr_pend_sv,		/* PendSV (used by the scheduler) */
	&isr_sys_tick,		/* SysTick */

	/*
	 * Ok I am REALLY tired of writing out mnemonics.
	 * Just have a look at include/arch/at91sam3x8e.h for details.
	 */
	&isr_rstc,
	&isr_rtc,
	&isr_rtt,
	&isr_wdt,
	&isr_pmc,
	&isr_efc0,
	&isr_efc1,
	&isr_uart,
	&isr_smc,
	NULL,			/* reserved */
	&isr_pioa,
	&isr_piob,
	&isr_pioc,
	&isr_piod,
	NULL,			/* reserved */
	NULL,			/* reserved */
	&isr_usart0,
	&isr_usart1,
	&isr_usart2,
	&isr_usart3,
	&isr_hsmci,
	&isr_twi0,
	&isr_twi1,
	&isr_spi0,
	NULL,			/* reserved */
	&isr_ssc,
	&isr_tc0,
	&isr_tc1,
	&isr_tc2,
	&isr_tc3,
	&isr_tc4,
	&isr_tc5,
	&isr_tc6,
	&isr_tc7,
	&isr_tc8,
	&isr_pwm,
	&isr_adc,
	&isr_dacc,
	&isr_dmac,
	&isr_uotghs,
	&isr_trng,
	&isr_emac,
	&isr_can0,
	&isr_can1,
};
