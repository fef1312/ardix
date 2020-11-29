/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/ringbuf.h>
#include <ardix/serial.h>

#include <arch/serial.h>
#include <arch/sched.h>

#include <stddef.h>

int serial_init(struct serial_interface *interface, long int baud)
{
	int err = -1;

	if (interface->id < 0)
		return -1; /* invalid interface */

	interface->baud = baud;

	interface->rx = ringbuf_create(SERIAL_BUFSZ);
	if (interface->rx == NULL)
		return -1;

	interface->tx = ringbuf_create(SERIAL_BUFSZ);
	if (interface->tx == NULL) {
		ringbuf_destroy(interface->rx);
		return -1;
	}

	err = arch_serial_init(interface);
	if (err) {
		ringbuf_destroy(interface->rx);
		ringbuf_destroy(interface->tx);
	}

	return err;
}

void serial_exit(struct serial_interface *interface)
{
	arch_serial_exit(interface);
	ringbuf_destroy(interface->rx);
	ringbuf_destroy(interface->tx);
	interface->id = -1;
}

ssize_t serial_read(void *dest, struct serial_interface *interface, size_t len)
{
	ssize_t ret;

	sched_atomic_enter();
	ret = (ssize_t)ringbuf_read(dest, interface->rx, len);
	sched_atomic_leave();

	return ret;
}

ssize_t serial_write(struct serial_interface *interface, const void *data, size_t len)
{
	ssize_t ret;

	sched_atomic_enter();
	ret = (ssize_t)ringbuf_write(interface->tx, data, len);
	sched_atomic_leave();

	return ret;
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
