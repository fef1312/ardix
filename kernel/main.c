/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/io.h>
#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/sched.h>

#include <config.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Core init routine.
 *
 * This is invoked from the startup code (usually) located in
 * arch/<architecture>/startup.c.
 */
int main(void)
{
	int err = kent_root_init();
	if (err != 0)
		return err;

	kevents_init();

	err = sched_init();
	if (err != 0)
		return err;

	err = devices_init();
	if (err != 0)
		return err;

	err = io_init();
	if (err != 0)
		return err;

	/* we should have a serial console now */

	printf("Ardix version " ARDIX_VERSION_STR "\n");
	printf("This is non-violent software, and there is NO WARRANTY.\n");
	printf("See <https://git.fef.moe/fef/ardix> for details.\n\n");

	/* TODO: The next big step is to write initd and fork to it here. */
	while (1)
		sleep(1000);
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
