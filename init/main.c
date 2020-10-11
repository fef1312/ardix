/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <arch/hardware.h>
#include <ardix/sched.h>
#include <stdint.h>
#include <stddef.h>

#define REG_PIOB_PER			(*(uint32_t *)0x400E1000U)
#define REG_PIOB_PDR			(*(uint32_t *)0x400E1004U)
#define REG_PIOB_OER			(*(uint32_t *)0x400E1010U)
#define REG_PIOB_ODR			(*(uint32_t *)0x400E1014U)
#define REG_PIOB_SODR			(*(uint32_t *)0x400E1030U)
#define REG_PIOB_CODR			(*(uint32_t *)0x400E1034U)

/**
 * Core init routine.
 *
 * This is invoked from the startup code (usually) located in
 * arch/<architecture>/startup.c.
 */
void do_bootstrap(void)
{
	bool on = true;
	uint32_t state = (1 << 27);
	uint32_t count = 0;

	sys_init();

	sched_init();

	REG_PIOB_OER |= state;
	REG_PIOB_PER |= state;

	/* we'll only let the LED flash for now */

	while (true) {
		if (count++ != 100000)
			continue;

		if (on) {
			REG_PIOB_SODR |= state;
			on = false;
		} else {
			REG_PIOB_CODR |= state;
			on = true;
		}

		count = 0;
	}
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
