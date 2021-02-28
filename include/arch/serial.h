/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/arch_include.h>

#include <ardix/serial.h>

int arch_serial_init(struct serial_device *dev);
void arch_serial_exit(struct serial_device *dev);

/**
 * Copy `buf` to a hardware buffer in the TX queue.
 * The transmission is performed asynchronously.
 *
 * @param dev: serial device to enqueue the buffer for
 * @param buf: raw buffer data
 * @param len: length of `buf`
 * @returns actual amount of bytes enqueued, or a negative error code on failure
 */
ssize_t arch_serial_write(struct serial_device *dev, const void *buf, size_t len);

#include ARCH_INCLUDE(serial.h)

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
