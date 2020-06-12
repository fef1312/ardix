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

#include <ardix/types.h>

/** CPU clock frequency in Hertz */
#define F_CPU 84000000UL

/** Pointer size in bytes */
#define PTRSIZE 4

/* The following three structs are basically just for reference */

/**
 * A software snapshot of all registers that have not been saved by automated
 * hardware routines on ISR entry.  Required for scheduling / context switching.
 */
struct reg_sw_snapshot {
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	/*
	 * lr is saved by hardware, but we need to store it twice
	 * because the ISR entry overwrites it
	 */
	uint32_t lr;	/* alias r14 */
};

/**
 * All registers that are automatically saved by hardware routines when entering
 * an ISR, in the correct order.
 */
struct reg_hw_snapshot {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r12;
	uint32_t lr;	/* alias r14 */
	uint32_t pc;	/* alias r15 */
	uint32_t psr;
};

/** Combination of `struct reg_sw_snapshot` and `struct reg_hw_snapshot`. */
struct reg_snapshot {
	struct reg_sw_snapshot sw;
	struct reg_hw_snapshot hw;
};

/*
 * Real-time Timer (RTT)
 */

/** Real-time Timer Mode Register */
#define REG_RTT_MR		(*(uint32_t *)0x400E1A30U)
/** Real-time Timer Restart bitmask (for `RTT_MR`) */
#define REG_RTTRST_MASK		((uint32_t)1 << 18)
/** Real-time Timer Increment Interrupt Enable bitmask (for `REG_RTT_MR`) */
#define REG_RTTINCIEN_MASK	((uint32_t)1 << 17)
/** Alarm Interrupt Enable bitmask (for `REG_RTT_MR`) */
#define REG_ALMIEN_MASK		((uint32_t)1 << 16)
/** Real-time Timer Prescaler Value */
#define REG_RTPRES		(*(uint16_t *)0x400E1A30U)

/** Real-time Timer Alarm Register */
#define REG_RTT_AR		(*(uint32_t *)0x400E1A34U)
/** Real-time Timer Alarm Value */
#define REG_ALMV		REG_RTT_AR

/** Real-time timer Value Register */
#define REG_RTT_VR		(*(uint32_t *)0x400E1A38U)
/** Current Real-time Value */
#define REG_CRTV		REG_RTT_VR

/** Real-time Timer Status Register */
#define REG_RTT_SR		(*(const uint32_t *)0x400E1A3CU)
/** Real-time Timer Increment bitmask (for `REG_RTT_SR`) */
#define REG_RTTINC_MASK		((uint32_t)1 << 1)
/** Real-time Timer Alarm Status bitmask (for `REG_RTT_SR`) */
#define REG_ALMS_MASK		((uint32_t)1)

/*
 * SysTick
 */

/** SysTick Control and Status Register */
#define REG_SYSTICK_CTRL			(*(uint32_t *)0xE000E010U)
/** SysTick CTRL COUNTFLAG bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_COUNTFLAG_MASK		((uint32_t)1 << 16)
/** SysTick CTRL CLKSOURCE bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_CLKSOURCE_MASK		((uint32_t)1 << 2)
/** SysTick CTRL exception request enable bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_TICKINT_MASK		((uint32_t)1 << 1)
/** SysTick CTRL enable bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_ENABLE_MASK		((uint32_t)1)

/** SysTick Reload Value Register */
#define REG_SYSTICK_LOAD		(*(uint32_t *)0xE000E014U)
#define REG_SYSTICK_LOAD_RELOAD_MASK	((uint32_t)0x00FFFFFFU)

/** SysTick Current Value Register */
#define REG_SYSTICK_VAL			(*(uint32_t *)0xE000E018U)
/** SysTick Current Value bitmask (for `REG_SYSTICK_VAL`) */
#define REG_SYSTICK_VAL_CURRENT_MASK	((uint32_t)0x00FFFFFFU)

/** SysTick Calibration Value Register */
#define REG_SYSTICK_CALIB		(*(uint32_t *)0xE000E01CU)
#define REG_SYSTICK_CALIB_NOREF_MASK	((uint32_t)1 << 31)
#define REG_SYSTICK_CALIB_SKEW_MASK	((uint32_t)1 << 30)
#define REG_SYSTICK_CALIB_TENMS_MASK	((uint32_t)0x00FFFFFFU)

/*
 * Real-time Clock (RTC)
 */

/** RTC Control Register */
#define REG_RTC_CR		(*(uint32_t *)0x400E1A60U)

/*
 * Universal Asynchronous Receiver Transceiver (UART)
 */

/** UART Control Register */
#define REG_UART_CR		(*(uint32_t *)0x400E0800U)
/** UART Control Register Reset Status Bits bitmask (for `REG_UART_CR`) */
#define REG_RSTSTA_MASK		((uint32_t)1 << 8)
/** UART Control Register Transmitter Disable bitmaask (for `REG_UART_CR`) */
#define REG_TXDIS_MASK		((uint32_t)1 << 7)
/** UART Control Register Transmitter Enable bitmaask (for `REG_UART_CR`) */
#define REG_TXEN_MASK		((uint32_t)1 << 6)
/** UART Control Register Receiver Disable bitmask (for `REG_UART_CR`) */
#define REG_RXDIS_MASK		((uint32_t)1 << 5)
/** UART Control Register Receiver Enable bitmask (for `REG_UART_CR`) */
#define REG_RXEN_MASK		((uint32_t)1 << 4)
/** UART Control Register Reset Transmitter bitmask (for `REG_UART_CR`) */
#define REG_RSTTX_MASK		((uint32_t)1 << 3)
/** UART Control Register Reset Receiver bitmask (for `REG_UART_CR`) */
#define REG_RSTRX_MASK		((uint32_t)1 << 2)

/** UART Mode Register */
#define REG_UART_MR		(*(uint32_t *)0x400E0804U)

/*
 * Nested Vectored Interrupt Controller
 */

/** System Control Block: Interrupt Control and State Register */
#define SCB_ICSR		(*(uint32_t *)0xE000ED04U)
/** ICSR PendSV set-pending bit bitmask (for `SCB_ICSR`) */
#define SCB_PENDSVSET_MASK	((uint32_t)1 << 28)
/** ICSR PendSV clear-pending bit bitmask (for `SCB_ICSR`) */
#define SCB_PENDSVCLR_MASK	((uint32_t)1 << 27)
/** ICSR SysTick exception set-pending bit bitmask (for `SCB_ICSR`) */
#define SCB_PENDSTSET_MASK	((uint32_t)1 << 26)
/** ICSR SysTick exception clear-pending bit bitmask (for `SCB_ICSR`) */
#define SCB_PENDSTCLR_MASK	((uint32_t)1 << 25)
/** ICSR Interrupt pending flag, excluding Faults bitmask (for `SCB_ICSR`) */
#define SCB_ISRPENDING_MASK	((uint32_t)1 << 22)
/**
 * ICSR bitmask for highest priority pending & enabled exception
 * (for `SCB_ICSR`).  The value is shifted 12 to the left.
 */
#define SCB_VECTPENDING_MASK	((uint32_t)0b1111111111u << 12)
/** ICSR bitmask for whether there are preempted active exceptions (for `SCB_ICSR`) */
#define SCB_RETTOBASE_MASK	((uint32_t)1 << 11)
/** ICSR active exception number bitmask (for `SCB_ICSR`) */
#define SCB_VECTACTIVE_MASK	((uint32_t)0b111111111u)
