/* See the end of this file for copyright, license, and warranty information. */

#include <stddef.h>
#include <toolchain.h>

/**
 * Default handler for unimplemented interrupts.
 * This will halt the system.
 */
void _stub_handler(void)
{
	while (1);
}

void handle_reset(void)		__weak __alias(_stub_handler);
void handle_nmi(void)		__weak __alias(_stub_handler);
void handle_hard_fault(void)	__weak __alias(_stub_handler);
void handle_mm_fault(void)	__weak __alias(_stub_handler);
void handle_bus_fault(void)	__weak __alias(_stub_handler);
void handle_usage_fault(void)	__weak __alias(_stub_handler);
void handle_svc(void)		__weak __alias(_stub_handler);
void handle_debug_mon(void)	__weak __alias(_stub_handler);
void handle_pend_sv(void)	__weak __alias(_stub_handler);
void handle_sys_tick(void)	__weak __alias(_stub_handler);

void irq_supc(void)	__weak __alias(_stub_handler);
void irq_rstc(void)	__weak __alias(_stub_handler);
void irq_rtc(void)	__weak __alias(_stub_handler);
void irq_rtt(void)	__weak __alias(_stub_handler);
void irq_wdt(void)	__weak __alias(_stub_handler);
void irq_pmc(void)	__weak __alias(_stub_handler);
void irq_efc0(void)	__weak __alias(_stub_handler);
void irq_efc1(void)	__weak __alias(_stub_handler);
void irq_uart(void)	__weak __alias(_stub_handler);
void irq_smc(void)	__weak __alias(_stub_handler);
void irq_pioa(void)	__weak __alias(_stub_handler);
void irq_piob(void)	__weak __alias(_stub_handler);
void irq_pioc(void)	__weak __alias(_stub_handler);
void irq_piod(void)	__weak __alias(_stub_handler);
void irq_usart0(void)	__weak __alias(_stub_handler);
void irq_usart1(void)	__weak __alias(_stub_handler);
void irq_usart2(void)	__weak __alias(_stub_handler);
void irq_usart3(void)	__weak __alias(_stub_handler);
void irq_hsmci(void)	__weak __alias(_stub_handler);
void irq_twi0(void)	__weak __alias(_stub_handler);
void irq_twi1(void)	__weak __alias(_stub_handler);
void irq_spi0(void)	__weak __alias(_stub_handler);
void irq_ssc(void)	__weak __alias(_stub_handler);
void irq_tc0(void)	__weak __alias(_stub_handler);
void irq_tc1(void)	__weak __alias(_stub_handler);
void irq_tc2(void)	__weak __alias(_stub_handler);
void irq_tc3(void)	__weak __alias(_stub_handler);
void irq_tc4(void)	__weak __alias(_stub_handler);
void irq_tc5(void)	__weak __alias(_stub_handler);
void irq_tc6(void)	__weak __alias(_stub_handler);
void irq_tc7(void)	__weak __alias(_stub_handler);
void irq_tc8(void)	__weak __alias(_stub_handler);
void irq_pwm(void)	__weak __alias(_stub_handler);
void irq_adc(void)	__weak __alias(_stub_handler);
void irq_dacc(void)	__weak __alias(_stub_handler);
void irq_dmac(void)	__weak __alias(_stub_handler);
void irq_uotghs(void)	__weak __alias(_stub_handler);
void irq_trng(void)	__weak __alias(_stub_handler);
void irq_emac(void)	__weak __alias(_stub_handler);
void irq_can0(void)	__weak __alias(_stub_handler);
void irq_can1(void)	__weak __alias(_stub_handler);

extern uint32_t _estack;

__section(.vectors) const void *exception_table[] = {
	&_estack,		/* initial SP value (stack grows down) */
	handle_reset,		/* reset vector */
	NULL,			/* reserved */
	handle_hard_fault,	/* hard fault */
	handle_mm_fault,	/* memory management fault */
	handle_bus_fault,	/* bus fault */
	handle_usage_fault,	/* usage fault */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	NULL,			/* reserved */
	handle_svc,		/* SVC call (used for syscalls) */
	handle_debug_mon,	/* reserved for debug */
	NULL,			/* reserved */
	handle_pend_sv,		/* PendSV (used by the scheduler) */
	handle_sys_tick,	/* SysTick */

	irq_supc,
	irq_rstc,
	irq_rtc,
	irq_rtt,
	irq_wdt,
	irq_pmc,
	irq_efc0,
	irq_efc1,
	irq_uart,
	irq_smc,
	NULL,			/* reserved */
	irq_pioa,
	irq_piob,
	irq_pioc,
	irq_piod,
	NULL,			/* reserved */
	NULL,			/* reserved */
	irq_usart0,
	irq_usart1,
	irq_usart2,
	irq_usart3,
	irq_hsmci,
	irq_twi0,
	irq_twi1,
	irq_spi0,
	NULL,			/* reserved */
	irq_ssc,
	irq_tc0,
	irq_tc1,
	irq_tc2,
	irq_tc3,
	irq_tc4,
	irq_tc5,
	irq_tc6,
	irq_tc7,
	irq_tc8,
	irq_pwm,
	irq_adc,
	irq_dacc,
	irq_dmac,
	irq_uotghs,
	irq_trng,
	irq_emac,
	irq_can0,
	irq_can1,
};

/*
 * This file is part of Ardix.
 * Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
