/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/hardware.h>

#include <ardix/io.h>
#include <ardix/kent.h>
#include <ardix/printk.h>
#include <ardix/sched.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

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
	uint32_t count = 0;
	unsigned int print_count = 0;

	REG_PIOB_OER = 1 << 27;
	REG_PIOB_PER = 1 << 27;
	REG_PIOB_CODR = 1 << 27;

	kent_root_init();

	sched_init();

	devices_init();

	io_init();

	while (true) {
		if (count++ != 1000000)
			continue;

		printk("hello, world (%u)!\n", print_count);

		print_count++;
		if (print_count % 2)
			REG_PIOB_CODR = 1 << 27;
		else
			REG_PIOB_SODR = 1 << 27;

		count = 0;
	}
}

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
