/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/file.h>
#include <ardix/io.h>
#include <ardix/serial.h>

#include <config.h>

struct file *kstdout;
struct file *kstdin;

int io_init(void)
{
	int ret;

	ret = serial_init(serial_default_device, CONFIG_SERIAL_BAUD);
	if (ret != 0)
		goto err_serial_init;

	kstdin = file_create(&serial_default_device->device, FILE_TYPE_PIPE, &ret);
	if (ret != 0)
		goto err_kstdin_create;

	kstdout = file_create(&serial_default_device->device, FILE_TYPE_PIPE, &ret);
	if (ret != 0)
		goto err_kstdout_create;

	goto out;

err_kstdout_create:
	file_put(kstdin);
err_kstdin_create:
	serial_exit(serial_default_device);
err_serial_init:
out:
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
