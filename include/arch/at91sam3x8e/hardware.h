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

/**
 * A software snapshot of all registers that have not been saved by automated
 * hardware routines on ISR entry.  Required for scheduling / context switching.
 */
struct reg_sw_snapshot {
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	/*
	 * lr is saved by hardware, but we need to store it twice
	 * because the ISR entry overwrites it
	 */
	void *lr;	/* alias r14 */
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
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
	void *lr;	/* alias r14 */
	void *pc;	/* alias r15 */
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
#define REG_RTT_RTTRST_BIT	((uint32_t)1 << 18)
/** Real-time Timer Increment Interrupt Enable bitmask (for `REG_RTT_MR`) */
#define REG_RTT_RTTINCIEN_BIT	((uint32_t)1 << 17)
/** Alarm Interrupt Enable bitmask (for `REG_RTT_MR`) */
#define REG_RTT_ALMIEN_BIT	((uint32_t)1 << 16)
/** Real-time Timer Prescaler Value */
#define REG_RTT_RTPRES		(*(uint16_t *)0x400E1A30U)

/** Real-time Timer Alarm Register */
#define REG_RTT_AR		(*(uint32_t *)0x400E1A34U)
/** Real-time Timer Alarm Value */
#define REG_RTT_AR_ALMV		REG_RTT_AR

/** Real-time timer Value Register */
#define REG_RTT_VR		(*(uint32_t *)0x400E1A38U)
/** Current Real-time Value */
#define REG_RTT_VR_CRTV		REG_RTT_VR

/** Real-time Timer Status Register */
#define REG_RTT_SR		(*(const uint32_t *)0x400E1A3CU)
/** Real-time Timer Increment bitmask (for `REG_RTT_SR`) */
#define REG_RTT_SR_RTTINC_BIT	((uint32_t)1 << 1)
/** Real-time Timer Alarm Status bitmask (for `REG_RTT_SR`) */
#define REG_RTT_SR_ALMS_BIT	((uint32_t)1)

/*
 * SysTick
 */

/** SysTick Control and Status Register */
#define REG_SYSTICK_CTRL			(*(uint32_t *)0xE000E010U)
/** SysTick CTRL COUNTFLAG bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_COUNTFLAG_BIT		((uint32_t)1 << 16)
/** SysTick CTRL CLKSOURCE bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_CLKSOURCE_BIT		((uint32_t)1 << 2)
/** SysTick CTRL exception request enable bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_TICKINT_BIT		((uint32_t)1 << 1)
/** SysTick CTRL enable bitmask (for `REG_SYSTICK_CTRL`) */
#define REG_SYSTICK_CTRL_ENABLE_BIT		((uint32_t)1)

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

/*
 * Enhanced Embedded Flash Controller (EEFC)
 */

/** EEFC 0 Flash Mode Register */
#define REG_EEFC0_FMR		(*(uint32_t *)0x400E0A00U)
/** EEFC 1 Flash Mode Register */
#define REG_EEFC1_FMR		(*(uint32_t *)0x400E0C00U)
/** EEFC Flash Access Mode bitmask */
#define REG_EEFC_FAM_BIT	((uint32_t)1 << 24)
/** EEFC Flash Wait State bitmask (number of cycles for read/write = FWS + 1) */
#define REG_EEFC_FWS_MASK	((uint32_t)0x7U << 8)
#define REG_EEFC_FWS_VAL(x) 	( ((uint32_t)(x) << 8) & REG_EEFC_FWS_MASK )
/** EEFC Ready Interrupt Enable bitmask */
#define REG_EEFC_FRDY_BIT	((uint32_t)1)

/*
 * Power Management Controller (PMC)
 */

/** PMC System Clock Enable Register */
#define REG_PMC_SCER			(*(uint32_t *)0x400E0600U)
/** PMC Programmable Clock 2 Output Enable bitmask (for `REG_PMC_SCER`) */
#define REG_PMC_SCER_PCK2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock 1 Output Enable bitmask (for `REG_PMC_SCER`) */
#define REG_PMC_SCER_PCK1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock 0 Output Enable bitmask (for `REG_PMC_SCER`) */
#define REG_PMC_SCER_PCK0_BIT		((uint32_t)1 << 8)
/** PMC Enable USB OTG Clock bitmask for UTMI (for `REG_PMC_SCER`) */
#define REG_PMC_SCER_UOTGCLK_BIT	((uint32_t)1 << 5)

/** PMC System Clock Disable Register */
#define REG_PMC_SCDR			(*(uint32_t *)0x400E0604U)
/** PMC Programmable Clock 2 Output Disable bitmask (for `REG_PMC_SCDR`) */
#define REG_PMC_SCDR_PCK2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock 1 Output Disable bitmask (for `REG_PMC_SCDR`) */
#define REG_PMC_SCDR_PCK1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock 0 Output Disable bitmask (for `REG_PMC_SCDR`) */
#define REG_PMC_SCDR_PCK0_BIT		((uint32_t)1 << 8)
/** PMC Disable USB OTG Clock bitmask for UTMI (for `REG_PMC_SCDR`) */
#define REG_PMC_SCDR_UOTGCLK_BIT	((uint32_t)1 << 5)

/** PMC System Clock Status Register */
#define REG_PMC_SCSR			(*(uint32_t *)0x400E0608U)
/** PMC Programmable Clock 2 Output Status bitmask */
#define REG_PMC_SCSR_PCK2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock 1 Output Status bitmask */
#define REG_PMC_SCSR_PCK1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock 0 Output Status bitmask */
#define REG_PMC_SCSR_PCK0_BIT		((uint32_t)1 << 8)
/** PMC USB OTG Clock Status bitmask */
#define REG_PMC_SCSR_UOTGCLK_BIT	((uint32_t)1 << 5)

/** PMC Peripheral Clock Enable Register 0 */
#define REG_PMC_PCER0			(*(uint32_t *)0x400E0610U)
/** PMC Peripheral Clock x Enable bitmask (2-31) */
#define REG_PMC_PCER0_PID(x)		((uint32_t)1 << x)

/** PMC Peripheral Clock Disable Register 0 */
#define REG_PMC_PCDR0			(*(uint32_t *)0x400E0614U)
/** PMC Peripheral Clock x Disable bitmask (2-31) */
#define REG_PMC_PCDR0_PID(x)		((uint32_t)1 << x)

/** PMC Peripheral Clock Status Register 0 */
#define REG_PMC_PCSR0			(*(const uint32_t *)0x400E0618U)
/** PMC Peripheral Clock x Status bitmask (2-31) */
#define REG_PMC_PCSR0_PID(x)		((uint32_t)1 << x)

/** PMC UTMI Clock Configuration Register */
#define REG_PMC_CKGR_UCKR			(*(uint32_t *)0x400E061CU)
/** PMC UTMI PLL Start-Up Time bitmask (shifted 20 to the left) */
#define REG_PMC_CKGR_UCKR_UPLLCOUNT_MASK	((uint32_t)0b1111 << 20)
/** PMC UTMI PLL Enable bitmask */
#define REG_PMC_CKGR_UCKR_UPLLEN_MASK		((uint32_t)1 << 16)

/** PMC Clock Generator Main Oscillator Register */
#define REG_CKGR_MOR			(*(uint32_t *)0x400E0620U)
/** PMC Clock Failure Detector Enable bitmask */
#define REG_CKGR_MOR_CFDEN_BIT		((uint32_t)1 << 25)
/** PMC Main Oscillator Selection bitmask */
#define REG_CKGR_MOR_MOSCSEL_BIT	((uint32_t)1 << 24)
/** PMC Main Oscillator Password bitmask */
#define REG_CKGR_MOR_KEY_MASK		((uint32_t)0xFF << 16)
#define REG_CKGR_MOR_KEY_VAL(x) \
	( ((uint32_t)(x) << 16) & REG_CKGR_MOR_KEY_MASK )
#define REG_CKGR_MOR_KEY_MAGIC		(0x37)
/** PMC Main Crystal Oscillator Start-up Time bitmask (<< 8, 8 bits, unit: clock cycles * 8) */
#define REG_CKGR_MOR_MOSCXTST_MASK	((uint32_t)0xFF << 8)
#define REG_CKGR_MOR_MOSCXTST_VAL(x) \
	( ((uint32_t)(x) << 8) & REG_CKGR_MOR_MOSCXTST_MASK )
/** PMC Main On-Chip RC Oscillator Frequency Selection bitmask (<< 4, 2 bits) */
#define REG_CKGR_MOR_MOSCRCF_MASK	((uint32_t)0b111 << 4)
#define REG_CKGR_MOR_MOSCRCF_VAL(x) \
	( ((uint32_t)(x) << 4) & REG_CKGR_MOR_MOSCRCF_MASK )
/** PMC Main On-Chip RC Oscillator Enable bitmask */
#define REG_CKGR_MOR_MOSCRCEN_BIT	((uint32_t)1 << 3)
/** PMC Main Crystal Oscillator Bypass bitmask */
#define REG_CKGR_MOR_MOSCXTBY_BIT	((uint32_t)1 << 1)
/** PMC Main Crystal Oscillator Enable bitmask */
#define REG_CKGR_MOR_MOSCXTEN_BIT	((uint32_t)1)

/** PMC Clock Generator Main Clock Frequency Register */
#define REG_CKGR_MCFR			(*(const uint32_t *)0x400E0624U)
/** PMC Main Clock Ready bitmask */
#define REG_CKGR_MCFR_MAINFRDY_BIT	((uint32_t)1 << 16)
/** PMC Main Clock Frequency bitmask (16-bit, equals amount of main clocks per 16 slow clocks) */
#define REG_CKGR_MCFR_MAINF_MASK	((uint32_t)0xFFFF)

/** PMC Clock Generator PLLA Register */
#define REG_CKGR_PLLAR			(*(uint32_t *)0x400E0628U)
/** PMC Clock Generator: Must be set to one when writing to CKGR_PLLAR bitmask */
#define REG_CKGR_PLLAR_ONE_BIT		((uint32_t)1 << 29)
/** PMC PLLA Multiplier bitmask (<< 16, 11 bits) */
#define REG_CKGR_PLLAR_MULA_MASK	((uint32_t)0x7FF << 16)
#define REG_CKGR_PLLAR_MULA_VAL(x) \
	( ((uint32_t)(x) << 16) & REG_CKGR_PLLAR_MULA_MASK )
/** PMC PLLA Counter bitmask (<< 8, 6 bits) */
#define REG_CKGR_PLLAR_PLLACOUNT_MASK	((uint32_t)0x3F << 8)
#define REG_CKGR_PLLAR_PLLACOUNT_VAL(x) \
	( ((uint32_t)(x) << 8) & REG_CKGR_PLLAR_PLLACOUNT_MASK )
/** PMC PLLA Divider bitmask (8 bits) */
#define REG_CKGR_PLLAR_DIVA_MASK	((uint32_t)0xFF)
#define REG_CKGR_PLLAR_DIVA_VAL(x) \
	((uint32_t)(x) & REG_CKGR_PLLAR_DIVA_MASK)

/** PMC Master Clock Register */
#define REG_PMC_MCKR			(*(uint32_t *)0x400E0630U)
/** PMC Master Clock UPLL Divisor by 2 bitmask */
#define REG_PMC_MCKR_UPLLDIV2_BIT	((uint32_t)1 << 13)
/** PMC Master Clock PLLA Divisor by 2 bitmask */
#define REG_PMC_MCKR_PLLADIV2_BIT	((uint32_t)1 << 12)
/** PMC Master Clock Processor Clock Prescaler bitmask (<< 4, 3 bits) */
#define REG_PMC_MCKR_PRES_MASK		((uint32_t)0x7 << 4)
#define REG_PMC_MCKR_PRES_VAL(x) \
	( ((uint32_t)(x) << 4) & REG_PMC_MCKR_PRES_MASK )
/** PMC Master Clock Source Selection bitmask (2 bits) */
#define REG_PMC_MCKR_CSS_MASK		((uint32_t)0x3)
#define REG_PMC_MCKR_CSS_VAL(x)		((uint32_t)(x) & REG_PMC_MCKR_CSS_MASK)

/** PMC USB Clock Register */
#define REG_PMC_USB			(*(uint32_t *)0x400E0638U)
/** PMC USB Divider for USB Clock bitmask (<< 8, 8 bits) */
#define REG_PMC_USB_USBDIV_MASK		((uint32_t)0xF << 8)
/** PMC USB Input Clock Selection bitmask */
#define REG_PMC_USB_USBS_MASK		((uint32_t)1)

/** PMC Programmable Clock Register */
#define REG_PMC_PCKx			(*(uint32_t *)0x400E0640U)
/** PMC Programmable Clock Prescaler bitmask (<< 4, 3 bits) */
#define REG_PMC_PCKx_PRES_MASK		((uint32_t)0x7 << 4)
#define REG_PMC_PCKx_PRES_VAL(x) \
	( ((uint32_t)(x) << 4) & REG_PMC_PCKx_PRES_MASK )
/** PMC Master Clock Source Selection bitmask (3 bits) */
#define REG_PMC_PCKx_CSS_MASK		((uint32_t)0x7)
#define REG_PMC_PCKx_CSS_VAL(x)		((uint32_t)(x) & REG_PMC_PCKx_CSS_MASK)

/** PMC Interrupt Enable Register */
#define REG_PMC_IER			(*(uint32_t *)0x400E0660U)
/** PMC Clock Failure Detector Event Interrupt Enable bitmask */
#define REG_PMC_IER_CFDEV_BIT		((uint32_t)1 << 18)
/** PMC Main On-Chip RC Status Interrupt Enable bitmask */
#define REG_PMC_IER_MOSCRCS_BIT		((uint32_t)1 << 17)
/** PMC Main Oscillator Selection Status Interrupt Enable bitmask */
#define REG_PMC_IER_MOSCSELS_BIT	((uint32_t)1 << 16)
/** PMC Programmable Clock Ready 2 Interrupt Enable bitmask */
#define REG_PMC_IER_PCKRDY2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock Ready 1 Interrupt Enable bitmask */
#define REG_PMC_IER_PCKRDY1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock Ready 0 Interrupt Enable bitmask */
#define REG_PMC_IER_PCKRDY0_BIT		((uint32_t)1 << 8)
/** PMC UTMI PLL Lock Interrupt Enable bitmask */
#define REG_PMC_IER_LOCKU_BIT		((uint32_t)1 << 6)
/** PMC Master Clock Ready Interrupt Enable bitmask */
#define REG_PMC_IER_MCKRDY_BIT		((uint32_t)1 << 3)
/** PMC PLLA Lock Interrupt Enable bitmask */
#define REG_PMC_IER_LOCKA_BIT		((uint32_t)1 << 1)
/** PMC Main Crystal Oscillator Status Interrupt Enable bitmask */
#define REG_PMC_IER_MOSCXTS_BIT		((uint32_t)1)

/** PMC Interrupt Disable Register */
#define REG_PMC_IDR			(*(uint32_t *)0x400E0664U)
/** PMC Clock Failure Detector Event Interrupt Disable bitmask */
#define REG_PMC_IDR_CFDEV_BIT		((uint32_t)1 << 18)
/** PMC Main On-Chip RC Status Interrupt Disable bitmask */
#define REG_PMC_IDR_MOSCRCS_BIT		((uint32_t)1 << 17)
/** PMC Main Oscillator Selection Status Interrupt Disable bitmask */
#define REG_PMC_IDR_MOSCSELS_BIT	((uint32_t)1 << 16)
/** PMC Programmable Clock Ready 2 Interrupt Disable bitmask */
#define REG_PMC_IDR_PCKRDY2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock Ready 1 Interrupt Disable bitmask */
#define REG_PMC_IDR_PCKRDY1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock Ready 0 Interrupt Disable bitmask */
#define REG_PMC_IDR_PCKRDY0_BIT		((uint32_t)1 << 8)
/** PMC UTMI PLL Lock Interrupt Disable bitmask */
#define REG_PMC_IDR_LOCKU_BIT		((uint32_t)1 << 6)
/** PMC Master Clock Ready Interrupt Disable bitmask */
#define REG_PMC_IDR_MCKRDY_BIT		((uint32_t)1 << 3)
/** PMC PLLA Lock Interrupt Disable bitmask */
#define REG_PMC_IDR_LOCKA_BIT		((uint32_t)1 << 1)
/** PMC Main Crystal Oscillator Status Interrupt Disable bitmask */
#define REG_PMC_IDR_MOSCXTS_BIT		((uint32_t)1)

/** PMC Status Register */
#define REG_PMC_SR			(*(const uint32_t *)0x400E0668U)
/** PMC Clock Failure Detector Event bitmask */
#define REG_PMC_SR_CFDEV_BIT		((uint32_t)1 << 18)
/** PMC Main On-Chip RC Oscillator Status bitmask */
#define REG_PMC_SR_MOSCRCS_BIT		((uint32_t)1 << 17)
/** PMC Main Oscillator Selection Status bitmask */
#define REG_PMC_SR_MOSCSELS_BIT		((uint32_t)1 << 16)
/** PMC Programmable Clock 2 Ready Status bitmask */
#define REG_PMC_SR_PCKRDY2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock 1 Ready Status bitmask */
#define REG_PMC_SR_PCKRDY1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock 0 Ready Status bitmask */
#define REG_PMC_SR_PCKRDY0_BIT		((uint32_t)1 << 8)
/** PMC UTMI PLL Lock Status bitmask */
#define REG_PMC_SR_LOCKU_BIT		((uint32_t)1 << 6)
/** PMC Master Clock Status bitmask */
#define REG_PMC_SR_MCKRDY_BIT		((uint32_t)1 << 3)
/** PMC PLLA Lock Status bitmask */
#define REG_PMC_SR_LOCKA_BIT		((uint32_t)1 << 1)
/** PMC Main Crystal Oscillator Status bitmask */
#define REG_PMC_SR_MOSCXTS_BIT		((uint32_t)1)

/** PMC Interrupt Mask Register */
#define REG_PMC_IMR			(*(const uint32_t *)0x400E066CU)
/** PMC Clock Failure Detector Event Interrupt Mask bitmask */
#define REG_PMC_IMR_CFDEV_BIT		((uint32_t)1 << 18)
/** PMC Main On-Chip RC Status Interrupt Mask bitmask */
#define REG_PMC_IMR_MOSCRCS_BIT		((uint32_t)1 << 17)
/** PMC Main Oscillator Selection Status Interrupt Mask bitmask */
#define REG_PMC_IMR_MOSCSELS_BIT	((uint32_t)1 << 16)
/** PMC Programmable Clock Ready 2 Interrupt Mask bitmask */
#define REG_PMC_IMR_PCKRDY2_BIT		((uint32_t)1 << 10)
/** PMC Programmable Clock Ready 1 Interrupt Mask bitmask */
#define REG_PMC_IMR_PCKRDY1_BIT		((uint32_t)1 << 9)
/** PMC Programmable Clock Ready 0 Interrupt Mask bitmask */
#define REG_PMC_IMR_PCKRDY0_BIT		((uint32_t)1 << 8)
/** PMC UTMI PLL Lock Interrupt Mask bitmask */
#define REG_PMC_IMR_LOCKU_BIT		((uint32_t)1 << 6)
/** PMC Master Clock Ready Interrupt Mask bitmask */
#define REG_PMC_IMR_MCKRDY_BIT		((uint32_t)1 << 3)
/** PMC PLLA Lock Interrupt Mask bitmask */
#define REG_PMC_IMR_LOCKA_BIT		((uint32_t)1 << 1)
/** PMC Main Crystal Oscillator Status Interrupt Mask bitmask */
#define REG_PMC_IMR_MOSCXTS_BIT		((uint32_t)1)

/* TODO */

/** PMC Write Protect Mode Register */
#define REG_PMC_WPMR			(*(uint32_t *)0x400E06E4U)
/** PMC Write Protect Key bitmask (<< 8, 24 bits, needs to be `0x504D43` ("PMC")) */
#define REG_PMC_WPMR_WPKEY_MASK		((uint32_t)0xFFFFFF << 8)
#define REG_PMC_WPMR_WPKEY_VAL(x) \
	( ((uint32_t)(x) << 8) & REG_PMC_WPMR_WPKEY_MASK )
#define REG_PMC_WPMR_WPKEY_MAGIC	(0x504D43) /* "PMC" in ASCII */
/** PMC Write Protect Enable bitmask */
#define REG_PMC_WPMR_WPEN_BIT		((uint32_t)1)

/** PMC Write Protect Status Register */
#define REG_PMC_WPSR			(*(uint32_t *)0x400E06E8U)
/** PMC Write Protect Violation Status bitmask */
#define REG_PMC_WPSR_WPVSRC_MASK	((uint32_t)0xFFFF << 8)
/** PMC Write Protect Violation Status bitmask (<< 8, 16 bits) */
#define REG_PMC_WPSR_WPVS_MASK		((uint32_t)1)
