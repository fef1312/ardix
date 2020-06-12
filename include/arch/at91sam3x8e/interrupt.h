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

#pragma once

/** Reset interrupt handler */
void isr_reset(void);
/** Non-maskable interrupt handler */
void isr_nmi(void);
/** Hard fault inerrupt handler */
void isr_hard_fault(void);
/** Memory management fault interrupt handler */
void isr_mem_fault(void);
/** Bus fault interrupt handler */
void isr_bus_fault(void);
/** Usage fault (illegal instruction) interrupt handler */
void isr_usage_fault(void);
/** SVC interrupt handler */
void isr_svc(void);
/** Debug handler (reserved) */
void isr_debug_mon(void);
/** Pending SV interrupt handler */
extern void isr_pend_sv(void);
/** SysTick interrupt handler */
void isr_sys_tick(void);

/** Supply Controller (0) interrupt handler */
void isr_supc(void);
/** Reset Controller (1) interrupt handler */
void isr_rstc(void);
/** Real-time Clock (2) interrupt handler */
void isr_rtc(void);
/** Real-time Timer (3) interrupt handler */
void isr_rtt(void);
/** Watchdog Timer (4) interrupt handler */
void isr_wdt(void);
/** Power Management Controller (5) interrupt handdler */
void isr_pmc(void);
/** Embedded Flash Controller 0 (6) interrupt handler */
void isr_efc0(void);
/** Embedded Flash Controller 1 (7) interrupt handler */
void isr_efc1(void);
/** Universal Asynchronous Receiver Transmitter (8) interrupt handler */
void isr_uart(void);
/** Static Memory Controller (9) interrupt handler */
void isr_smc(void);
/** Parallel I/O Controller A (11) interrupt handler */
void isr_pioa(void);
/** Parallel I/O Controller B (12) interrupt handler */
void isr_piob(void);
/** Parallel I/O Controller C (13) interrupt handler */
void isr_pioc(void);
/** Parallel I/O Controller D (14) interrupt handler */
void isr_piod(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 0 (17) interrupt handler */
void isr_usart0(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 1 (18) interrupt handler */
void isr_usart1(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 2 (19) interrupt handler */
void isr_usart2(void);
/** Universal Synchronous/Asynchronous Receiver Transmitter 3 (20) interrupt handler */
void isr_usart3(void);
/** Multimedia Card Interface (21) interrupt handler */
void isr_hsmci(void);
/** Two-Wire Interface 0 (22) interrupt handler */
void isr_twi0(void);
/** Two-Wire Interface 1 (23) interrupt handler */
void isr_twi1(void);
/** Serial Peripheral Interface 0 (24) interrupt handler */
void isr_spi0(void);
/** Synchronous Serial Controller (26) interrupt handler */
void isr_ssc(void);
/** Timer/Counter 0 (27) interrupt handler */
void isr_tc0(void);
/** Timer/Counter 1 (28) interrupt handler */
void isr_tc1(void);
/** Timer/Counter 2 (29) interrupt handler */
void isr_tc2(void);
/** Timer/Counter 3 (30) interrupt handler */
void isr_tc3(void);
/** Timer/Counter 4 (31) interrupt handler */
void isr_tc4(void);
/** Timer/Counter 5 (32) interrupt handler */
void isr_tc5(void);
/** Timer/Counter 6 (33) interrupt handler */
void isr_tc6(void);
/** Timer/Counter 7 (34) interrupt handler */
void isr_tc7(void);
/** Timer/Counter 8 (35) interrupt handler */
void isr_tc8(void);
/** Pulse Width Modulation Controller (36) interrupt handler */
void isr_pwm(void);
/** Analog to Digital Converter Controller (37) interrupt handler */
void isr_adc(void);
/** Digital to Analog Converter Controller (38) interrupt handler */
void isr_dacc(void);
/** Direct Memory Access Controller (39) interrupt handler */
void isr_dmac(void);
/** USB OTG High Speed (40) interrupt handler */
void isr_uotghs(void);
/** True Random Number Generator (41) interrupt handler */
void isr_trng(void);
/** Ethernet MAC (42) interrupt handler */
void isr_emac(void);
/** Controller Area Network 0 (43) interrupt handler */
void isr_can0(void);
/** Controller Area Network 1 (44) interrupt handler */
void isr_can1(void);

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
	IRQNO_POIA		= 11,
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
