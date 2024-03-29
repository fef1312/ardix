/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atomic.h>
#include <ardix/dma.h>
#include <ardix/io.h>
#include <ardix/malloc.h>
#include <ardix/ringbuf.h>
#include <ardix/serial.h>
#include <ardix/types.h>

#include <arch/hardware.h>
#include <arch/interrupt.h>
#include <arch-generic/serial.h>

#include <errno.h>
#include <stddef.h>
#include <string.h>

struct arch_serial_device arch_serial_default_device = {
	.device = {
		.rx = NULL,
		.id = 0,
		.baud = 0,
	},
	.txbuf = NULL,
};
struct serial_device *serial_default_device = &arch_serial_default_device.device;

int arch_serial_init(struct serial_device *dev)
{
	if (dev->baud <= 0 || dev->id != 0)
		return -1;

	/* enable peripheral clock for UART (which has peripheral id 8) */
	PMC->PMC_PCER0 |= PMC_PCER0_PID8;

	/* ensure the PIO controller is turned off on the serial pins */
	PIOA->PIO_PDR = PIO_PDR_P8 | PIO_PDR_P9;

	/* configure peripheral DMA controller */
	UART->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTEN;

	/* reset & disable rx and tx */
	UART->UART_CR = UART_CR_RXDIS | UART_CR_RSTRX
		      | UART_CR_TXDIS | UART_CR_RSTTX;

	/* no parity, normal mode */
	UART->UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;

	/* From Atmel Datasheet: baud rate = MCK / (UART_BRGR * 16) */
	UART->UART_BRGR = UART_BRGR_CD(( SystemCoreClock / (uint32_t)dev->baud ) >> 4);

	/* choose the events we want an interrupt on */
	UART->UART_IDR = 0xffffffff; /* make sure all interrupts are disabled first */
	UART->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

	NVIC_EnableIRQ(UART_IRQn);

	/* enable receiver and transmitter */
	UART->UART_CR = UART_CR_RXEN | UART_CR_TXEN;

	return 0;
}

void arch_serial_exit(struct serial_device *dev)
{
	if (dev->id != 0)
		return;

	/* disable receiver and transmitter */
	UART->UART_CR = UART_CR_RXDIS | UART_CR_TXDIS;

	NVIC_DisableIRQ(UART_IRQn);

	/* disable peripheral clock for UART (PID is taken from Atmel Datasheet, Section 9.1 */
	PMC->PMC_PCDR0 = PMC_PCDR0_PID8;

	dev->id = -1;
}

ssize_t arch_serial_write(struct serial_device *dev, const void *buf, size_t len)
{
	struct arch_serial_device *arch_dev = to_arch_serial_device(dev);

	if (len > 0xffff)
		return -E2BIG;

	if (arch_dev->txbuf != NULL)
		return -EBUSY;

	struct dmabuf *dmabuf = dmabuf_create(&dev->device, len);
	if (dmabuf == NULL)
		return -ENOMEM;

	memcpy(dmabuf->data, buf, len);

	arch_dev->txbuf = dmabuf;
	UART->UART_TPR = (uintptr_t)dmabuf->data;
	UART->UART_TCR = dmabuf->len;
	UART->UART_IER = UART_IER_TXBUFE;

	return (ssize_t)len;
}

void irq_uart(void)
{
	__irq_enter();

	uint8_t tmp;
	uint32_t state = UART->UART_SR;

	/* RX has received a byte, store it into the ring buffer */
	if (state & UART_SR_RXRDY) {
		tmp = (uint8_t)UART->UART_RHR;
		ringbuf_write(arch_serial_default_device.device.rx, &tmp, sizeof(tmp));

		device_kevent_create_and_dispatch(&serial_default_device->device,
						  DEVICE_KEVENT_RX);
	}

	/* UART_TCR has reached zero */
	if (state & UART_SR_TXBUFE) {
		dmabuf_put(arch_serial_default_device.txbuf);
		arch_serial_default_device.txbuf = NULL;

		UART->UART_IDR = UART_IDR_TXBUFE;

		device_kevent_create_and_dispatch(&serial_default_device->device,
						  DEVICE_KEVENT_TX);
	}

	/* check for error conditions */
	if ((state & UART_SR_OVRE) || (state & UART_SR_FRAME)) {
		UART->UART_CR = UART_CR_RSTSTA;
		device_kevent_create_and_dispatch(
			&serial_default_device->device,
			DEVICE_KEVENT_RX | DEVICE_KEVENT_TX | DEVICE_KEVENT_ERR
		);
	}

	__irq_leave();
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
