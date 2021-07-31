/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/io.h>
#include <ardix/serial.h>

int io_init(void)
{
	int ret;

	ret = serial_init(serial_default_device, CONFIG_SERIAL_BAUD);

	return ret;
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
