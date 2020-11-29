/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <ardix/types.h>

/** Current (volatile) system frequency in Hertz. */
extern uint32_t sys_core_clock;

/** Pointer size in bytes */
#define PTRSIZE 4

/**
 * A software snapshot of all registers that have not been saved by automated
 * hardware routines on IRQ entry.  Required for scheduling / context switching.
 */
struct reg_sw_snapshot {
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	/*
	 * lr is saved by hardware, but we need to store it twice
	 * because the IRQ entry overwrites it
	 */
	void *lr;	/* alias r14 */
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
};

/**
 * All registers that are automatically saved by hardware routines when entering
 * an IRQ, in the correct order.
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
 * Parallel Input/Output Controller (PIO)
 */

/*
 * put these into the `n` parameter below. PIOA - PIOF are exactly the same,
 * except that their register addresses are offset by 0x200 respectively.
 */
#define PIOA 0
#define PIOB 1
#define PIOC 2
#define PIOD 3

/** PIO Controller n Enable Register (requires `REG_PIOA_WPMR` to be cleared) */
#define REG_PIO_PER(n)			(*(uint32_t *)(0x400E0E00U + 0x200U * n))
/** PIO Controller n Disable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_PDR(n)			(*(uint32_t *)(0x400E0E04U + 0x200U * n))
/** PIO Controller n Status Register */
#define REG_PIO_PSR(n)			(*(uint32_t *)(0x400E0E08U + 0x200U * n))
/** PIO Controller n Output Enable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_OER(n)			(*(uint32_t *)(0x400E0E10U + 0x200U * n))
/** PIO Controller n Output Disable Register (newwds `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_ODR(n)			(*(uint32_t *)(0x400E0E14U + 0x200U * n))
/** PIO Controller n Output Status Register */
#define REG_PIO_OSR(n)			(*(uint32_t *)(0x400E0E18U + 0x200U * n))
/** PIO Controller n Input Filter Enable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_IFER(n)			(*(uint32_t *)(0x400E0E20U + 0x200U * n))
/** PIO Controller A Input Filter Disable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_IFDR(n)			(*(uint32_t *)(0x400E0E24U + 0x200U * n))
/** PIO Controller n Input Filter Status Register */
#define REG_PIO_IFSR(n)			(*(uint32_t *)(0x400E0E28U + 0x200U * n))
/** PIO Controller n Set Output Data Register */
#define REG_PIO_SODR(n)			(*(uint32_t *)(0x400E0E30U + 0x200U * n))
/** PIO Controller n Clear Output Data Register */
#define REG_PIO_CODR(n)			(*(uint32_t *)(0x400E0E34U + 0x200U * n))
/** PIO Controller n Output Data Status Register */
#define REG_PIO_ODSR(n)			(*(uint32_t *)(0x400E0E38U + 0x200U * n))
/** PIO Controller n Pin Data Status Register */
#define REG_PIO_PDSR(n)			(*(uint32_t *)(0x400E0E3CU + 0x200U * n))
/** PIO Controller n Interrupt Enable Register */
#define REG_PIO_IER(n)			(*(uint32_t *)(0x400E0E40U + 0x200U * n))
/** PIO Controller n Interrupt Disable Register */
#define REG_PIO_IDR(n)			(*(uint32_t *)(0x400E0E44U + 0x200U * n))
/** PIO Controller n Interrupt Mask Register */
#define REG_PIO_IMR(n)			(*(uint32_t *)(0x400E0E48U + 0x200U * n))
/** PIO Controller n Interrupt Status Register */
#define REG_PIO_ISR(n)			(*(uint32_t *)(0x400E0E4CU + 0x200U * n))
/** PIO Controller n Multi-driver Enable Register */
#define REG_PIO_MDER(n)			(*(uint32_t *)(0x400E0E50U + 0x200U * n))
/** PIO Controller n Multi-dtiver Disable Register */
#define REG_PIO_MDDR(n)			(*(uint32_t *)(0x400E0E54U + 0x200U * n))
/** PIO Controller n Multi-driver Status Register */
#define REG_PIO_MDSR(n)			(*(uint32_t *)(0x400E0E58U + 0x200U * n))
/** PIO Controller n Pull Up Disable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_PUDR(n)			(*(uint32_t *)(0x400E0E60U + 0x200U * n))
/** PIO Controller n Pull Up Enable Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_PUER(n)			(*(uint32_t *)(0x400E0E64U + 0x200U * n))
/** PIO Controller n Pull Up Status Register */
#define REG_PIO_PUSR(n)			(*(uint32_t *)(0x400E0E68U + 0x200U * n))
/** PIO Controller n Peripheral AB Select Register (requires `REG_PIO_WPMR` to be cleared) */
#define REG_PIO_ABSR(n)			(*(uint32_t *)(0x400E0E70U + 0x200U * n))

/* TODO: It's 3 am I really don't feel like doint the rest of these endless registers */

/** PIO Controller n Write Protect Mode Register */
#define REG_PIO_WPMR(n)			(*(uint32_t *)(0x400E0EE4U + 0x200U * n))
#define REG_PIO_WPMR_KEY		((uint32_t)0x50494FU << 8)
#define REG_PIO_WPMR_WPEN_VAL(val)	((uint32_t)val | REG_PIO_WPMR_KEY)
/** PIO Controller n Write Protect Status Register */
#define REG_PIO_WPSR(n)			(*(uint32_t *)0x400E0EE8U + 0x200U * n)
#define REG_PIO_WPSR_WPVSRC_MASK	((uint32_t)0x00FFFF00U)
#define REG_PIO_WPSR_WPVSRC_VAL(val)	((val & REG_PIO_WPSR_WPVSRC_MASK) >> 8)
#define REG_PIO_WPSR_WPVS_MASK		((uint32_t)1)
#define REG_PIO_WPST_WPVS_VAL(val)	(val & 1U)

/*
 * Universal Asynchronous Receiver Transceiver (UART)
 */

/** UART Control Register */
#define REG_UART_CR			(*(uint32_t *)0x400E0800U)
/** UART Control Register Reset Status Bits bitmask (for `REG_UART_CR`) */
#define REG_UART_CR_RSTSTA_MASK		((uint32_t)1 << 8)
/** UART Control Register Transmitter Disable bitmaask (for `REG_UART_CR`) */
#define REG_UART_CR_TXDIS_MASK		((uint32_t)1 << 7)
/** UART Control Register Transmitter Enable bitmaask (for `REG_UART_CR`) */
#define REG_UART_CR_TXEN_MASK		((uint32_t)1 << 6)
/** UART Control Register Receiver Disable bitmask (for `REG_UART_CR`) */
#define REG_UART_CR_RXDIS_MASK		((uint32_t)1 << 5)
/** UART Control Register Receiver Enable bitmask (for `REG_UART_CR`) */
#define REG_UART_CR_RXEN_MASK		((uint32_t)1 << 4)
/** UART Control Register Reset Transmitter bitmask (for `REG_UART_CR`) */
#define REG_UART_CR_RSTTX_MASK		((uint32_t)1 << 3)
/** UART Control Register Reset Receiver bitmask (for `REG_UART_CR`) */
#define REG_UART_CR_RSTRX_MASK		((uint32_t)1 << 2)

/** UART Mode Register */
#define REG_UART_MR			(*(uint32_t *)0x400E0804U)
/** UART Mode Register Channel Mode bitmask (for `REG_UART_MR`) */
#define REG_UART_MR_CHMODE_MASK		((uint32_t)3 << 14)
/** Mask and shift the value of `REG_UART_MR` to get `CHMODE` */
#define REG_UART_MR_CHMODE_VAL(val)	((val & REG_UART_MR_CHMODE_MASK) >> 14)
/** UART Mode Register Parity Type bitmask (for `REG_UART_MR`) */
#define REG_UART_MR_PAR_MASK		((uint32_t)7 << 9)
/** Mask and shift the value of `REG_UART_MR` to get `PAR`*/
#define REG_UART_MR_PAR_VAL(val)	((val & REG_UART_MR_PAR_MASK) >> 9)
/** Normal mode */
#define REG_UART_MR_CHMODE_NORMAL		((uint32_t)0)
/** Automatic Echo */
#define REG_UART_MR_CHMODE_AUTOMATIC		((uint32_t)1 << 14)
/** Local Loopback */
#define REG_UART_MR_CHMODE_LOCAL_LOOPBACK	((uint32_t)2 << 14)
/** Remote Loopback */
#define REG_UART_MR_CHMODE_REMOTE_LOOPBACK	((uint32_t)3 << 14)

#define REG_UART_MR_PAR_EVEN		((uint32_t)0)
#define REG_UART_MR_PAR_ODD		((uint32_t)1 << 9)
#define REG_UART_MR_PAR_SPACE		((uint32_t)2 << 9)
#define REG_UART_MR_PAR_MARK		((uint32_t)3 << 9)
#define REG_UART_MR_PAR_NO		((uint32_t)4 << 9)

/** UART Interrupt Enable Register */
#define REG_UART_IER			(*(uint32_t *)0x400E0808U)
/** UART IER Enable RXRDY Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_RXRDY_MASK		((uint32_t)1 << 0)
/** UART IER Enable TXRDY Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_TXRDY_MASK		((uint32_t)1 << 1)
/** UART IER Enable End of Receive Transfer Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_ENDRX_MASK		((uint32_t)1 << 3)
/** UART IER Enable End of Transmit Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_ENDTX_MASK		((uint32_t)1 << 4)
/** UART IER Enable Overrun Error Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_OVRE_MASK		((uint32_t)1 << 5)
/** UART IER Enable Framing Error Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_FRAME_MASK		((uint32_t)1 << 6)
/** UART IER Enable Parity Error Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_PARE_MASK		((uint32_t)1 << 7)
/** UART IER Enable TXEMPTY Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_TXEMPTY_MASK	((uint32_t)1 << 9)
/** UART IER Enable Buffer Empty Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_TXBUFE_MASK	((uint32_t)1 << 11)
/** UART IER Enable Buffer Full Interrupt bitmask (for `REG_UART_IER`) */
#define REG_UART_IER_RXBUFF_MASK	((uint32_t)1 << 12)

/** UART Interrupt Disable Register */
#define REG_UART_IDR			(*(uint32_t *)0x400E080CU)
/** UART IDR Disable RXRDY Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_RXRDY_MASK		((uint32_t)1 << 0)
/** UART IDR Disable TXRDY Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_TXRDY_MASK		((uint32_t)1 << 1)
/** UART IDR Disable End of Receive Transfer Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_ENDRX_MASK		((uint32_t)1 << 3)
/** UART IDR Disable End of Transmit Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_ENDTX_MASK		((uint32_t)1 << 4)
/** UART IDR Disable Overrun Error Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_OVRE_MASK		((uint32_t)1 << 5)
/** UART IDR Disable Framing Error Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_FRAME_MASK		((uint32_t)1 << 6)
/** UART IDR Disable Parity Error Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_PARE_MASK		((uint32_t)1 << 7)
/** UART IDR Disable TXEMPTY Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_TXEMPTY_MASK	((uint32_t)1 << 9)
/** UART IDR Disable Buffer Empty Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_TXBUFE_MASK	((uint32_t)1 << 11)
/** UART IDR Disable Buffer Full Interrupt bitmask (for `REG_UART_IDR`) */
#define REG_UART_IDR_RXBUFF_MASK	((uint32_t)1 << 12)

/** UART Interrupt Mask Register */
#define REG_UART_IMR			(*(uint32_t *)0x400E0810U)
/** UART IMR Mask RXRDY Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_RXRDY_MASK		((uint32_t)1 << 0)
/** UART IMR Mask TXRDY Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_TXRDY_MASK		((uint32_t)1 << 1)
/** UART IMR Mask End of Receive Transfer Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_ENDRX_MASK		((uint32_t)1 << 3)
/** UART IMR Mask End of Transmit Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_ENDTX_MASK		((uint32_t)1 << 4)
/** UART IMR Mask Overrun Error Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_OVRE_MASK		((uint32_t)1 << 5)
/** UART IMR Mask Framing Error Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_FRAME_MASK		((uint32_t)1 << 6)
/** UART IMR Mask Parity Error Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_PARE_MASK		((uint32_t)1 << 7)
/** UART IMR Mask TXEMPTY Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_TXEMPTY_MASK	((uint32_t)1 << 9)
/** UART IMR Mask Buffer Empty Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_TXBUFE_MASK	((uint32_t)1 << 11)
/** UART IMR Mask Buffer Full Interrupt bitmask (for `REG_UART_IMR`) */
#define REG_UART_IMR_RXBUFF_MASK	((uint32_t)1 << 12)

/** UART Status Register */
#define REG_UART_SR			(*(uint32_t *)0x400E0814U)
/** UART SR Receiver Ready bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_RXRDY_MASK		((uint32_t)1 << 0)
/** UART SR Transmitter Ready bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_TXRDY_MASK		((uint32_t)1 << 1)
/** UART SR End of Receiver Transfer bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_ENDRX_MASK		((uint32_t)1 << 3)
/** UART SR End of Transmitter Transfer bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_ENDTX_MASK		((uint32_t)1 << 4)
/** UART SR Overrun Error bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_OVRE_MASK		((uint32_t)1 << 5)
/** UART SR Framing Error bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_FRAME_MASK		((uint32_t)1 << 6)
/** UART SR Parity Error bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_PARE_MASK		((uint32_t)1 << 7)
/** UART SR TXEMPTY bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_TXEMPTY_MASK	((uint32_t)1 << 9)
/** UART SR Buffer Empty bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_TXBUFE_MASK		((uint32_t)1 << 11)
/** UART SR Buffer Full bitmask (for `REG_UART_SR`) */
#define REG_UART_SR_RXBUFF_MASK		((uint32_t)1 << 12)

/** UART Receiver Holding Register */
#define REG_UART_RHR			(*(uint8_t *)0x400E0818U)
/** UART Receiver Holding Register */
#define REG_UART_THR			(*(uint8_t *)0x400E081CU)

/** UART Baud Rate Generator Register */
#define REG_UART_BRGR			(*(uint16_t *)0x400E0820U)

/* UART PDC Area */

/** UART PDC Receive Pointer Register */
#define REG_UART_PDC_RPR		(*(uint32_t *)0x400E0900U)
/** UART PDC Receive Counter Register */
#define REG_UART_PDC_RCR		(*(uint16_t *)0x400E0904U)
/** UART PDC Transmit Pointer Register */
#define REG_UART_PDC_TPR		(*(uint32_t *)0x400E0908U)
/** UART PDC Transmit Counter Register */
#define REG_UART_PDC_TCR		(*(uint16_t *)0x400E090CU)
/** UART PDC Receive Next Pointer Register */
#define REG_UART_PDC_RNPR		(*(uint32_t *)0x400E0910U)
/** UART PDC Receive Next Counter Register */
#define REG_UART_PDC_RNCR		(*(uint32_t *)0x400E0914U)
/** UART PDC Transmit Next Pointer Register */
#define REG_UART_PDC_TNPR		(*(uint32_t *)0x400E0918U)
/** UART PDC Transmit Next Counter Register */
#define REG_UART_PDC_TNCR		(*(uint32_t *)0x400E091CU)

/** UART PDC Transfer Control Register */
#define REG_UART_PDC_PTCR		(*(uint32_t *)0x400E0920U)
#define REG_UART_PDC_PTCR_TXTDIS_MASK	((uint32_t)1 << 9)
#define REG_UART_PDC_PTCR_TXTEN_MASK	((uint32_t)1 << 8)
#define REG_UART_PDC_PTCR_RXTDIS_MASK	((uint32_t)1 << 1)
#define REG_UART_PDC_PTCR_RXTEN_MASK	((uint32_t)1 << 0)

/** UART PDC Transfer Status Register */
#define REG_UART_PDC_PTSR		(*(uint32_t *)(0x400E0800U + 0x124C))

/*
 * Nested Vectored Interrupt Controller
 */

/** NVIC Interrupt Set-Enable Registers (2 total) */
#define REG_NVIC_ISER(n)		(((uint32_t *)0xE000E100U)[n])
/** NVIC Interrupt Clear-Enable Registers (2 total) */
#define REG_NVIC_ICER(n)		(((uint32_t *)0xE000E180U)[n])
/** NVIC Interrupt Set-Pending Registers (2 total) */
#define REG_NVIC_ISPR(n)		(((uint32_t *)0xE000E200U)[n])
/** NVIC Interrupt Clear-Pending Registers (2 total) */
#define REG_NVIC_ICPR(n)		(((uint32_t *)0xE000E280U)[n])
/** NVIC Interrupt Active Bit Registers (2 total) */
#define REG_NVIC_IABR(n)		(((uint32_t *)0xE000E300U)[n])
/** NVIC Interrupt Priority Registers (8 total) */
#define REG_NVIC_IPR(n)			((uint32_t *)0xE000E400U)[n])
/** NVIC Software Trigger Interrupt Register */
#define REG_NVIC_STIR			(*(uint32_t *)0xE000EF00U)

/** System Control Block: Interrupt Control and State Register */
#define REG_SCB_ICSR			(*(uint32_t *)0xE000ED04U)
/** ICSR PendSV set-pending bit bitmask */
#define REG_SCB_ICSR_PENDSVSET_BIT	((uint32_t)1 << 28)
/** ICSR PendSV clear-pending bit bitmask */
#define REG_SCB_ICSR_PENDSVCLR_BIT	((uint32_t)1 << 27)
/** ICSR SysTick exception set-pending bit bitmask */
#define REG_SCB_ICSR_PENDSTSET_BIT	((uint32_t)1 << 26)
/** ICSR SysTick exception clear-pending bit bitmask */
#define REG_SCB_ICSR_PENDSTCLR_BIT	((uint32_t)1 << 25)
/** ICSR Interrupt pending flag, excluding Faults bitmask */
#define REG_SCB_ICSR_IRQPENDING_BIT	((uint32_t)1 << 22)
/**
 * ICSR bitmask for highest priority pending & enabled exception
 * (for `SCB_ICSR`).  The value is shifted 12 to the left.
 */
#define REG_SCB_ICSR_VECTPENDING_MASK	((uint32_t)0b1111111111u << 12)
#define REG_SCB_ICSR_VECTPENDING_VAL(x) \
	( ((uint32_t)(x) << 12) & REG_SCB_ICSR_VECTPENDING_MASK )
/** ICSR bitmask for whether there are preempted active exceptions */
#define REG_SCB_ICSR_RETTOBASE_BIT	((uint32_t)1 << 11)
/** ICSR active exception number bitmask */
#define REG_SCB_ICSR_VECTACTIVE_MASK	((uint32_t)0b111111111u)
#define REG_SCB_ICSR_VECTACTIVE_VAL(x) \
	((uint32_t)(x) & REG_SCB_ICSR_VECTACTIVE_MASK)

/*
 * System Control Block:
 * Application Interrupt and Reset Control Register
 */

/** Application Interrupt and Reset Control Register (AIRCR) */
#define REG_SCB_AIRCR			(*(uint32_t *)0xE000ED0CU)
#define REG_SCB_AIRCR_VECTKEY_MASK	((uint32_t)0xFFFF << 16)
#define REG_SCB_AIRCR_VECTKEY_VAL(x) \
	( ((uint32_t)(x) << 16) & REG_SCB_AIRCR_VECTKEY_MASK )
#define REG_SCB_AIRCR_VECTKEY_MAGIC	0x05FA
#define REG_SCB_AIRCR_ENDIANESS_BIT	((uint32_t)1 << 15)
#define REG_SCB_AIRCR_PRIGROUP_MASK	((uint32_t)0x3 << 8)
#define REG_SCB_AIRCR_PRIGROUP_VAL(x) \
	( ((uint32_t)(x) << 8) & REG_SCB_AIRCR_PRIGROUP_MASK )
#define REG_SCB_AIRCR_SYSRESETREQ_BIT	((uint32_t)1 << 2)

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
#define REG_PMC_WPMR_WPKEY_MAGIC	(0x504D43 << 8) /* "PMC" in ASCII */
#define REG_PMC_WPMR_WPEN_VAL(x)	((uint32_t)(x) | REG_PMC_WPMR_WPKEY_MAGIC)

/** PMC Write Protect Status Register */
#define REG_PMC_WPSR			(*(uint32_t *)0x400E06E8U)
/** PMC Write Protect Violation Status bitmask */
#define REG_PMC_WPSR_WPVSRC_MASK	((uint32_t)0xFFFF << 8)
/** PMC Write Protect Violation Status bitmask (<< 8, 16 bits) */
#define REG_PMC_WPSR_WPVS_MASK		((uint32_t)1)

/*
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
