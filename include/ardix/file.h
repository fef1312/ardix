/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/mutex.h>
#include <ardix/types.h>

enum file_type {
	FILE_TYPE_REGULAR,
	FILE_TYPE_PIPE,
};

struct file {
	struct kent kent;
	int fd;
	loff_t pos;
	struct mutex lock;
	struct device *device;
	enum file_type type;
};

struct file *file_create(struct device *dev, enum file_type type, int *err);

struct file *file_get(int fd);
void file_put(struct file *file);

ssize_t file_write(struct file *file, const void *buf, size_t len);
ssize_t file_read(void *buf, struct file *file, size_t len);

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
