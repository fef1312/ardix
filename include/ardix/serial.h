/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <ardix/types.h>
#include <ardix/ringbuf.h>
#include <toolchain.h>

/** Low-level hardware operations implemented by the serial driver. */
struct serial_operations {
	/** Begin transmission at the specified baud rate. */
	int (*begin)(struct serial_interface *serial, long int baud);
	/** Read from the hardware input buffer and store it into the rx ring buffer */
	ssize_t (*read)(uint8_t *dest, struct serial_interface *serial, size_t len);
	/** Read from the tx ring buffer and store it into the hardware output buffer */
	ssize_t (*write)(struct serial_interface *serial, const uint8_t *src, size_t len);
};

struct serial_interface {
	struct ringbuf *rx;
	struct ringbuf *tx;
	long int baud;
	struct serial_operations operations;
};

/** The default serial console (this is where printk outputs to) */
extern struct serial_interface *serial_default_interface;

/**
 * Initialize a serial interface.
 *
 * @param interface: The serial interface.
 * @param baud: The baud rate (bits/second).
 * @param buflen: The respective length of the read and write buffer.
 * @returns 0 on success, a negative number otherwise.
 */
int serial_init(struct serial_interface *interface, long int baud, size_t buflen);

/**
 * Flush all buffers (if possible) and close the serial interface.
 *
 * @param interface: The serial interface.
 */
void serial_exit(struct serial_interface *interface);

/**
 * Read from the serial buffer.
 *
 * @param dest: Where to store the received data.
 * @param interface: The serial interface to read data from.
 * @param len: The maximum amount of bytes to read.
 * @returns The actual amount of bytes read.
 */
ssize_t serial_read(uint8_t *dest, struct serial_interface *interface, size_t len);

/**
 * Write data to the serial buffer.
 *
 * @param interface: The serial interface to write data to.
 * @param data: The data to write.
 * @param len: The length of `data`.
 * @returns The actual amount of bytes written.
 */
ssize_t serial_write(struct serial_interface *interface, const uint8_t *data, size_t len);

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