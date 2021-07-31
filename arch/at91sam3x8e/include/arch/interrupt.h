/* See the end of this file for copyright, license, and warranty information. */

#pragma once

/** Reset interrupt handler */
void irq_reset(void);
/** Non-maskable interrupt handler */
void irq_nmi(void);
/** Hard fault inerrupt handler */
void irq_hard_fault(void);
/** Memory management fault interrupt handler */
void irq_mem_fault(void);
/** Bus fault interrupt handler */
void irq_bus_fault(void);
/** Usage fault (illegal instruction) interrupt handler */
void irq_usage_fault(void);
/** SVC interrupt handler */
void irq_svc(void);
/** Debug handler (reserved) */
void irq_debug_mon(void);
/** Pending SV interrupt handler */
void irq_pend_sv(void);
/** SysTick interrupt handler */
void irq_sys_tick(void);

/** Supply Controller (0) interrupt handler */
void irq_supc(void);
/** Reset Controller (1) interrupt handler */
void irq_rstc(void);
/** Real-time Clock (2) interrupt handler */
void irq_rtc(void);
/** Real-time Timer (3) interrupt handler */
void irq_rtt(void);
/** Watchdog Timer (4) interrupt handler */
void irq_wdt(void);
/** Power Management Controller (5) interrupt handler */
void irq_pmc(void);
/** Embedded Flash Controller 0 (6) interrupt handler */
void irq_efc0(void);
/** Embedded Flash Controller 1 (7) interrupt handler */
void irq_efc1(void);
/** Universal Asynchronous Receiver Transmitter (8) interrupt handler */
void irq_uart(void);
/** Static Memory Controller (9) interrupt handler */
void irq_smc(void);
/** Parallel I/O Controller A (11) interrupt handler */
void irq_pioa(void);
/** Parallel I/O Controller B (12) interrupt handler */
void irq_piob(void);
/** Parallel I/O Controller C (13) interrupt handler */
void irq_pioc(void);
/** Parallel I/O Controller D (14) interrupt handler */
void irq_piod(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 0 (17) interrupt handler */
void irq_usart0(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 1 (18) interrupt handler */
void irq_usart1(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 2 (19) interrupt handler */
void irq_usart2(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 3 (20) interrupt handler */
void irq_usart3(void);
/** Multimedia Card Interface (21) interrupt handler */
void irq_hsmci(void);
/** Two-Wire Interface 0 (22) interrupt handler */
void irq_twi0(void);
/** Two-Wire Interface 1 (23) interrupt handler */
void irq_twi1(void);
/** Serial Peripheral Interface 0 (24) interrupt handler */
void irq_spi0(void);
/** Synchronous Serial Controller (26) interrupt handler */
void irq_ssc(void);
/** Timer/Counter 0 (27) interrupt handler */
void irq_tc0(void);
/** Timer/Counter 1 (28) interrupt handler */
void irq_tc1(void);
/** Timer/Counter 2 (29) interrupt handler */
void irq_tc2(void);
/** Timer/Counter 3 (30) interrupt handler */
void irq_tc3(void);
/** Timer/Counter 4 (31) interrupt handler */
void irq_tc4(void);
/** Timer/Counter 5 (32) interrupt handler */
void irq_tc5(void);
/** Timer/Counter 6 (33) interrupt handler */
void irq_tc6(void);
/** Timer/Counter 7 (34) interrupt handler */
void irq_tc7(void);
/** Timer/Counter 8 (35) interrupt handler */
void irq_tc8(void);
/** Pulse Width Modulation Controller (36) interrupt handler */
void irq_pwm(void);
/** Analog to Digital Converter Controller (37) interrupt handler */
void irq_adc(void);
/** Digital to Analog Converter Controller (38) interrupt handler */
void irq_dacc(void);
/** Direct Memory Access Controller (39) interrupt handler */
void irq_dmac(void);
/** USB OTG High Speed (40) interrupt handler */
void irq_uotghs(void);
/** True Random Number Generator (41) interrupt handler */
void irq_trng(void);
/** Ethernet MAC (42) interrupt handler */
void irq_emac(void);
/** Controller Area Network 0 (43) interrupt handler */
void irq_can0(void);
/** Controller Area Network 1 (44) interrupt handler */
void irq_can1(void);

/**
 * Interrupt numbers for sam3x8e
 */
enum irqno {
	IRQNO_NMI		= -14,
	IRQNO_MEM_FAULT		= -12,
	IRQNO_BUS_FAULT		= -11,
	IRQNO_USAGE_FAULT	= -10,
	IRQNO_SVC_CALL		= -5,
	IRQNO_DEBUG		= -4,
	IRQNO_PEND_SV		= -2,
	IRQNO_SYS_TICK		= -1,

	IRQNO_SUPC		= 0,
	IRQNO_RSTC		= 1,
	IRQNO_RTC		= 2,
	IRQNO_RTT		= 3,
	IRQNO_WDT		= 4,
	IRQNO_PMC		= 5,
	IRQNO_EFC0		= 6,
	IRQNO_EFC1		= 7,
	IRQNO_UART		= 8,
	IRQNO_SMC		= 9,
	IRQNO_PIOA		= 11,
	IRQNO_PIOB		= 12,
	IRQNO_PIOC		= 13,
	IRQNO_PIOD		= 14,
	IRQNO_USART0		= 17,
	IRQNO_USART1		= 18,
	IRQNO_USART2		= 19,
	IRQNO_USART3		= 20,
	IRQNO_HSMCI		= 21,
	IRQNO_TWI0		= 22,
	IRQNO_TWI1		= 23,
	IRQNO_SPI0		= 24,
	IRQNO_SSC		= 26,
	IRQNO_TC0		= 27,
	IRQNO_TC1		= 28,
	IRQNO_TC2		= 29,
	IRQNO_TC3		= 30,
	IRQNO_TC4		= 31,
	IRQNO_TC5		= 32,
	IRQNO_TC6		= 33,
	IRQNO_TC7		= 34,
	IRQNO_TC8		= 35,
	IRQNO_PWM		= 36,
	IRQNO_ADC		= 37,
	IRQNO_DACC		= 38,
	IRQNO_DMAC		= 39,
	IRQNO_UOTGHS		= 40,
	IRQNO_TRNG		= 41,
	IRQNO_EMAC		= 42,
	IRQNO_CAN0		= 43,
	IRQNO_CAN1		= 44,
};

void arch_irq_enable(enum irqno irqno);

void arch_irq_disable(enum irqno irqno);

void arch_irq_invoke(enum irqno irqno);

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
