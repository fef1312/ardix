/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/device.h>
#include <ardix/types.h>
#include <ardix/ringbuf.h>

#include <toolchain.h>

#ifndef CONFIG_SERIAL_BAUD
/** serial baud rate */
#define CONFIG_SERIAL_BAUD 115200
#endif

#ifndef SERIAL_BUFSZ
/** size of a serial I/O buffer in bytes */
#define SERIAL_BUFSZ 256
#endif

struct serial_device {
	struct device device;
	struct ringbuf *rx;
	long int baud;
	int id;
};

/** The default serial console (this is where printk outputs to) */
extern struct serial_device *serial_default_device;

/**
 * Initialize a serial device.
 *
 * @param dev: serial device
 * @param baud: baud rate (bits/sec)
 * @returns 0 on success, a negative number otherwise.
 */
int serial_init(struct serial_device *dev, long int baud);

/** Flush all buffers (if possible) and close the serial device. */
void serial_exit(struct serial_device *dev);

/**
 * Read from the serial buffer.
 *
 * @param dest: where to write received data
 * @param dev: serial device to read from
 * @param len: amount of bytes to read
 * @returns actual amount of bytes read
 */
ssize_t serial_read(void *dest, struct serial_device *dev, size_t len);

/**
 * Write data to the serial buffer.
 *
 * @param dev: serial device to write to
 * @param data: where to read data from
 * @param len: amount of bytes to write
 * @returns actual amount of bytes written
 */
ssize_t serial_write(struct serial_device *dev, const void *data, size_t len);

/*
 * This file is part of Ardix.
 * Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
