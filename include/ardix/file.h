/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/kevent.h>
#include <ardix/mutex.h>
#include <ardix/types.h>

enum file_type {
	FILE_TYPE_REGULAR,
	FILE_TYPE_PIPE,
};

struct file {
	struct kent kent;
	int fd;
	off_t pos;
	struct mutex lock;
	struct device *device;
	enum file_type type;
};

struct file *file_create(struct device *dev, enum file_type type, int *err);

struct file *file_get(int fd);
void file_put(struct file *file);

ssize_t file_write(struct file *file, const void *buf, size_t len);
ssize_t file_read(void *buf, struct file *file, size_t len);

enum file_kevent_flags {
	FILE_KEVENT_READ		= (1 << 0),
	FILE_KEVENT_WRITE		= (1 << 1),
	FILE_KEVENT_CLOSE		= (1 << 2),
	FILE_KEVENT_EOF			= (1 << 3),
	FILE_KEVENT_UNLOCK		= (1 << 4),
};

struct file_kevent {
	struct kevent kevent;
	enum file_kevent_flags flags;
};

__always_inline struct file *kevent_to_file(struct kevent *event)
{
	return container_of(event->kent.parent, struct file, kent);
}

__always_inline struct file_kevent *kevent_to_file_kevent(struct kevent *event)
{
	return container_of(event, struct file_kevent, kevent);
}

struct file_kevent *file_kevent_create(struct file *f, enum file_kevent_flags flags);

void file_kevent_create_and_dispatch(struct file *f, enum file_kevent_flags flags);

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
