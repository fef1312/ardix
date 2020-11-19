/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/serial.h>
#include <ardix/types.h>
#include <ardix/ringbuf.h>

#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>
#include <arch/serial.h>

int arch_serial_init(struct serial_interface *interface)
{
	if (interface->baud <= 0 || interface->id != 0)
		return -1;

	/* enable peripheral clock for UART (which has peripheral id 8) */
	REG_PMC_PCER0 = REG_PMC_PCER0_PID(8);

	/* UART is multiplexed with PIOA, so we need to enable that controller first */
	REG_PIO_WPMR(PIOA) = REG_PIO_WPMR_WPEN_VAL(0); /* turn write protection off */
	REG_PIO_PER(PIOA) = (1 << 8) | (1 << 9);
	REG_PIO_WPMR(PIOA) = REG_PIO_WPMR_WPEN_VAL(1); /* turn write protection back on */

	/* reset & disable rx and tx */
	REG_UART_CR = REG_UART_CR_RXDIS_MASK | REG_UART_CR_RSTRX_MASK
		    | REG_UART_CR_TXDIS_MASK | REG_UART_CR_RSTTX_MASK;

	/* no parity, normal mode */
	REG_UART_MR = REG_UART_MR_PAR_NO | REG_UART_MR_CHMODE_NORMAL;

	/* From Atmel Datasheet: baud rate = MCK / (REG_UART_BRGR * 16) */
	REG_UART_BRGR = (uint16_t)((sys_core_clock / (uint32_t)interface->baud) >> 4);

	/* choose the events we want an interrupt on */
	REG_UART_IDR = 0xFFFFFFFF; /* make sure all interrupts are disabled first */
	REG_UART_IER = REG_UART_IER_RXRDY_MASK | REG_UART_IER_OVRE_MASK | REG_UART_IER_FRAME_MASK;

	arch_irq_enable(IRQNO_UART);

	/* enable transmitter and receiver */
	REG_UART_CR = REG_UART_CR_RXEN_MASK | REG_UART_CR_TXEN_MASK;

	return 0;
}

void arch_serial_exit(struct serial_interface *interface)
{
	if (interface->id != 0)
		return;

	REG_UART_CR = REG_UART_CR_RXDIS_MASK | REG_UART_CR_TXDIS_MASK;

	arch_irq_disable(IRQNO_UART);

	/* disable I/O line */
	REG_PIO_WPMR(PIOA) = REG_PIO_WPMR_WPEN_VAL(1);
	REG_PIO_PER(PIOA) = (1 << 8) | (1 << 9);
	REG_PIO_WPMR(PIOA) = REG_PIO_WPMR_WPEN_VAL(0);

	/* disable peripheral clock for UART */
	REG_PMC_PCDR0 = REG_PMC_PCDR0_PID(8);
}

int arch_serial_op_begin(struct serial_interface *interface)
{
	/* TODO */
	return -1;
}

ssize_t arch_serial_op_read(struct serial_interface *interface)
{
	/* TODO */
	return 0;
}

ssize_t arch_serial_op_write(struct serial_interface *interface)
{
	/* TODO */
	return 0;
}

struct serial_operations arch_serial_operations = {
	.begin = &arch_serial_op_begin,
	.read = &arch_serial_op_read,
	.write = &arch_serial_op_write,
};

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
