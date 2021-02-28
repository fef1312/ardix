/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/dma.h>
#include <ardix/serial.h>
#include <ardix/util.h>

#ifndef CONFIG_ARCH_SERIAL_BUFSZ
#define CONFIG_ARCH_SERIAL_BUFSZ 32
#endif /* CONFIG_ARCH_SERIAL_BUFSZ */

/** Architecture-specific extension of `struct serial_device` */
struct arch_serial_device {
	/** should always match REG_UART_PDC_TPR */
	struct dmabuf *tx_current;
	/** should always match REG_UART_PDC_TNPR */
	struct dmabuf *tx_next;

	struct serial_device device;
};

/**
 * Cast a `struct serial_device` out to a `struct arch_serialdevice`.
 *
 * @param ptr: The `struct serial_device *` to cast out from.
 * @returns The containing `struct arch_serialdevice *`.
 */
#define to_arch_serial_device(ptr) container_of(ptr, struct arch_serial_device, device)

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
