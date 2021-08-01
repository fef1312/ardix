/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/device.h>
#include <ardix/file.h>
#include <ardix/malloc.h>

#include <config.h>
#include <errno.h>
#include <stddef.h>

static struct file *fdtab[CONFIG_NFILE];
static MUTEX(fdtab_lock);

static void file_destroy(struct kent *kent)
{
	struct file *file = container_of(kent, struct file, kent);

	mutex_lock(&fdtab_lock);
	fdtab[file->fd] = NULL;
	mutex_unlock(&fdtab_lock);

	free(file);
}

struct file *file_create(struct device *device, enum file_type type, int *err)
{
	struct file *f;
	struct file **slot;
	int fd;

	mutex_lock(&fdtab_lock);
	for (fd = 0; fd < CONFIG_NFILE; fd++) {
		slot = &fdtab[fd];
		if (*slot == NULL)
			break;
	}
	if (*slot != NULL) {
		*err = -EMFILE;
		mutex_unlock(&fdtab_lock);
		return NULL;
	}

	f = malloc(sizeof(*f));
	if (f == NULL) {
		*err = -ENOMEM;
		mutex_unlock(&fdtab_lock);
		return NULL;
	}
	*slot = f;
	mutex_unlock(&fdtab_lock);

	f->kent.parent = &device->kent;
	f->kent.destroy = file_destroy;
	kent_init(&f->kent);

	f->fd = fd;
	f->device = device;
	f->pos = 0;
	f->type = type;
	mutex_init(&f->lock);

	return f;
}

struct file *file_get(int fd)
{
	struct file *f = NULL;

	if (fd < CONFIG_NFILE) {
		mutex_lock(&fdtab_lock);
		f = fdtab[fd];
		if (f != NULL)
			kent_get(&f->kent);
		mutex_unlock(&fdtab_lock);
	}

	return f;
}

void file_put(struct file *f)
{
	kent_put(&f->kent);
}

ssize_t file_read(void *buf, struct file *file, size_t len)
{
	ssize_t ret;
	mutex_lock(&file->lock);

	ret = file->device->read(buf, file->device, len, file->pos);
	if (file->type == FILE_TYPE_REGULAR && ret > 0)
		file->pos += ret;

	mutex_unlock(&file->lock);
	return ret;
}

ssize_t file_write(struct file *file, const void *buf, size_t len)
{
	ssize_t ret;
	mutex_lock(&file->lock);

	ret = file->device->write(file->device, buf, len, file->pos);
	if (file->type == FILE_TYPE_REGULAR && ret > 0)
		file->pos += ret;

	mutex_unlock(&file->lock);
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
