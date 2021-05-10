/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/dma.h>
#include <ardix/io.h>
#include <ardix/malloc.h>
#include <ardix/ringbuf.h>
#include <ardix/serial.h>
#include <ardix/types.h>

#include <arch/at91sam3x8e/hardware.h>
#include <arch/at91sam3x8e/interrupt.h>
#include <arch/serial.h>

#include <errno.h>
#include <stddef.h>
#include <string.h>

struct arch_serial_device arch_serial_default_device = {
	.tx_current = NULL,
	.tx_next = NULL,
	.device = {
		.rx = NULL,
		.id = 0,
		.baud = 0,
	},
};
struct serial_device *serial_default_device = &arch_serial_default_device.device;

int arch_serial_init(struct serial_device *dev)
{
	if (dev->baud <= 0 || dev->id != 0)
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
	REG_UART_BRGR = (uint16_t)(( sys_core_clock / (uint32_t)dev->baud ) >> 4);

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

void arch_serial_exit(struct serial_device *dev)
{
	if (dev->id != 0)
		return;

	/* disable receiver and transmitter */
	REG_UART_CR = REG_UART_CR_RXDIS_MASK | REG_UART_CR_TXDIS_MASK;

	arch_irq_disable(IRQNO_UART);

	/* disable peripheral clock for UART (PID is taken from Atmel Datasheet, Section 9.1 */
	REG_PMC_PCDR0 = REG_PMC_PCDR0_PID(8);

	dev->id = -1;
}

ssize_t arch_serial_write(struct serial_device *dev, const void *buf, size_t len)
{
	int ret;
	struct dmabuf *dmabuf = dmabuf_create(&dev->device, len);
	if (dmabuf == NULL)
		return -ENOMEM;

	memcpy(dmabuf->data, buf, len);
	ret = serial_write_dma(dev, dmabuf);
	dmabuf_put(dmabuf);
	return ret;
}

ssize_t serial_write_dma(struct serial_device *dev, struct dmabuf *buf)
{
	uint16_t len;
	struct arch_serial_device *arch_dev = to_arch_serial_device(dev);

	dmabuf_get(buf);

	if (arch_dev->tx_next != NULL)
		return -EBUSY;

	if (buf->len >= 0xffff)
		len = 0xffff;
	else
		len = (uint16_t)buf->len;

	if (arch_dev->tx_current == NULL) {
		arch_dev->tx_current = buf;
		REG_UART_PDC_TPR = (uint32_t)buf->data;
		REG_UART_PDC_TCR = len;
		/* we weren't transmitting, so the interrupt was masked */
		REG_UART_IER = REG_UART_IER_ENDTX_MASK;
	} else {
		arch_dev->tx_next = buf;
		REG_UART_PDC_TNPR = (uint32_t)buf->data;
		REG_UART_PDC_TNCR = len;
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
		ringbuf_write(arch_serial_default_device.device.rx, &tmp, sizeof(tmp));
	}

	/* REG_UART_PDC_TCR has reached zero */
	if (state & REG_UART_SR_ENDTX_MASK) {
		if (arch_serial_default_device.tx_current != NULL)
			dmabuf_put(arch_serial_default_device.tx_current);

		/* DMA automatically does this to the actual hardware registers */
		arch_serial_default_device.tx_current = arch_serial_default_device.tx_next;
		arch_serial_default_device.tx_next = NULL;

		if (arch_serial_default_device.tx_current == NULL)
			REG_UART_IDR = REG_UART_IDR_ENDTX_MASK;
	}

	/* check for error conditions */
	if ((state & REG_UART_SR_OVRE_MASK) || (state & REG_UART_SR_FRAME_MASK)) {
		/* TODO: write some proper error handling routines ffs */
		REG_UART_CR = REG_UART_CR_RSTSTA_MASK;
	}
}

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
