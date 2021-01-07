/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/io.h>
#include <ardix/malloc.h>
#include <ardix/ringbuf.h>
#include <ardix/serial.h>
#include <ardix/string.h>
#include <ardix/types.h>

#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>
#include <arch/serial.h>

#include <errno.h>
#include <stddef.h>

struct arch_serial_interface arch_serial_default_interface = {
	.tx_current = NULL,
	.tx_next = NULL,
	.interface = {
		.rx = NULL,
		.id = 0,
		.baud = 0,
	},
};
struct serial_interface *serial_default_interface = &arch_serial_default_interface.interface;

int arch_serial_init(struct serial_interface *interface)
{
	if (interface->baud <= 0 || interface->id != 0)
		return -1;

	/* enable peripheral clock for UART (which has peripheral id 8) */
	REG_PMC_PCER0 |= REG_PMC_PCER0_PID(8);

	/* ensure the PIO controller is turned off on the serial pins */
	REG_PIO_PDR(PIOA) = (1 << 8) | (1 << 9);

	/* configure peripheral DMA controller */
	REG_UART_PDC_PTCR = REG_UART_PDC_PTCR_RXTDIS_MASK | REG_UART_PDC_PTCR_TXTEN_MASK;

	/* reset & disable rx and tx */
	REG_UART_CR = REG_UART_CR_RXDIS_MASK | REG_UART_CR_RSTRX_MASK
		    | REG_UART_CR_TXDIS_MASK | REG_UART_CR_RSTTX_MASK;

	/* no parity, normal mode */
	REG_UART_MR = REG_UART_MR_PAR_NO | REG_UART_MR_CHMODE_NORMAL;

	/* From Atmel Datasheet: baud rate = MCK / (REG_UART_BRGR * 16) */
	REG_UART_BRGR = (uint16_t)(( sys_core_clock / (uint32_t)interface->baud ) >> 4);

	/* choose the events we want an interrupt on */
	REG_UART_IDR = 0xFFFFFFFF; /* make sure all interrupts are disabled first */
	REG_UART_IER = REG_UART_IER_RXRDY_MASK
		     | REG_UART_IER_OVRE_MASK
		     | REG_UART_IER_FRAME_MASK;

	arch_irq_enable(IRQNO_UART);

	/* enable receiver and transmitter */
	REG_UART_CR = REG_UART_CR_RXEN_MASK | REG_UART_CR_TXEN_MASK;

	return 0;
}

void arch_serial_exit(struct serial_interface *interface)
{
	if (interface->id != 0)
		return;

	/* disable receiver and transmitter */
	REG_UART_CR = REG_UART_CR_RXDIS_MASK | REG_UART_CR_TXDIS_MASK;

	arch_irq_disable(IRQNO_UART);

	/* disable peripheral clock for UART (PID is taken from Atmel Datasheet, Section 9.1 */
	REG_PMC_PCDR0 = REG_PMC_PCDR0_PID(8);

	interface->id = -1;
}

ssize_t arch_serial_write(struct serial_interface *interface, const void *buf, size_t len)
{
	struct arch_serial_buffer *arch_buf = NULL;
	struct arch_serial_interface *arch_iface = to_arch_serial_interface(interface);

	if (arch_iface->tx_next != NULL)
		return -EBUSY;

	if (len >= (1 << 16)) /* DMA uses 16-bit counters */
		len = 0xffff;

	arch_buf = malloc(sizeof(*arch_buf) + len);
	if (arch_buf == NULL)
		return -ENOMEM;

	memcpy(&arch_buf->data[0], buf, len);
	arch_buf->len = (uint16_t)len;

	if (arch_iface->tx_current == NULL) {
		arch_iface->tx_current = arch_buf;
		REG_UART_PDC_TPR = (uint32_t)&arch_buf->data[0];
		REG_UART_PDC_TCR = arch_buf->len;
		/* we weren't transmitting, so the interrupt was masked */
		REG_UART_IER = REG_UART_IER_ENDTX_MASK;
	} else {
		arch_iface->tx_next = arch_buf;
		REG_UART_PDC_TNPR = (uint32_t)&arch_buf->data[0];
		REG_UART_PDC_TNCR = arch_buf->len;
	}

	return (ssize_t)len;
}

void irq_uart(void)
{
	uint8_t tmp;
	uint32_t state = REG_UART_SR;

	/* RX has received a byte, store it into the ring buffer */
	if (state & REG_UART_SR_RXRDY_MASK) {
		tmp = REG_UART_RHR;
		ringbuf_write(arch_serial_default_interface.interface.rx, &tmp, sizeof(tmp));
	}

	/* REG_UART_PDC_TCR has reached zero */
	if (state & REG_UART_SR_ENDTX_MASK) {
		/* this might be NULL but that's ok because free() tolerates that */
		free(arch_serial_default_interface.tx_current);

		/* DMA automatically does this to the actual hardware registers */
		arch_serial_default_interface.tx_current = arch_serial_default_interface.tx_next;
		arch_serial_default_interface.tx_next = NULL;

		if (arch_serial_default_interface.tx_current == NULL)
			REG_UART_IDR = REG_UART_IDR_ENDTX_MASK;
	}

	/* check for error conditions */
	if ((state & REG_UART_SR_OVRE_MASK) || (state & REG_UART_SR_FRAME_MASK)) {
		/* TODO: write some proper error handling routines ffs */
		REG_UART_CR = REG_UART_CR_RSTSTA_MASK;
	}
}

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
