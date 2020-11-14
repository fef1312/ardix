/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/serial.h>
#include <ardix/types.h>
#include <ardix/ringbuf.h>

#include <arch/hardware.h>
#include <arch/serial.h>

int arch_serial_init(struct serial_interface *interface)
{
	/* TODO */
	return -1;
}

void arch_serial_exit(struct serial_interface *interface)
{
	/* TODO */
}

int arch_serial_op_begin(struct serial_interface *interface)
{
	/* TODO */
	return -1;
}

ssize_t arch_serial_op_read(struct serial_interface *interface)
{
	/* TODO */
	return 0;
}

ssize_t arch_serial_op_write(struct serial_interface *interface)
{
	/* TODO */
	return 0;
}

struct serial_operations arch_serial_operations = {
	.begin = &arch_serial_op_begin,
	.read = &arch_serial_op_read,
	.write = &arch_serial_op_write,
};

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
