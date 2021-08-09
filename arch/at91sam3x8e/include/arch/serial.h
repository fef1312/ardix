/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch-generic/serial.h>
#include <ardix/dma.h>
#include <ardix/serial.h>
#include <ardix/util.h>

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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
