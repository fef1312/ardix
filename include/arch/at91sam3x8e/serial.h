/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <ardix/serial.h>
#include <ardix/types.h>
#include <ardix/util.h>

#include <stdbool.h>

#ifndef CONFIG_ARCH_SERIAL_BUFSZ
#define CONFIG_ARCH_SERIAL_BUFSZ 32
#endif /* CONFIG_ARCH_SERIAL_BUFSZ */

/** Architecture-specific extension of `struct serial_interface` */
struct arch_serial_interface {
	struct serial_interface interface;

	/*
	 * two hardware buffers; one is for being written to while the other one can be read from
	 * by the hardware w/out interfering with each other.  `arch_serial_hwbuf_rotate()` is
	 * responsible for writing this to the respective hardware register and swapping them out.
	 * The platform's buffer length registers only allow 16-byte numbers, so we can save some
	 * memory by not using `size_t`
	 */
	uint8_t txbuf[CONFIG_ARCH_SERIAL_BUFSZ];
	/** hardware has finished sending the current buffer and ready for a swap */
	bool hw_txrdy;
};

/**
 * Cast a `struct serial_interface` out to a `struct arch_serial_interface`.
 *
 * @param ptr: The `struct serial_interface *` to cast out from.
 * @returns The containing `struct arch_serial_interface *`.
 */
#define to_arch_serial_interface(ptr) container_of(ptr, struct arch_serial_interface, interface)

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
