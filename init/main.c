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
void main(void)
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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
