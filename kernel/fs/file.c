/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/device.h>
#include <ardix/file.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>

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

	kfree(file);
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

	f = kmalloc(sizeof(*f), MEM_KERNEL);
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

struct io_device_kevent_extra {
	struct file *file;
	struct task *task;
	enum device_kevent_flags flags;
};

static int io_device_kevent_listener(struct kevent *event, void *_extra)
{
	struct io_device_kevent_extra *extra = _extra;

	struct device *device = kevent_to_device(event);
	if (device != extra->file->device)
		return KEVENT_CB_NONE;

	struct device_kevent *device_kevent = kevent_to_device_kevent(event);
	if ((device_kevent->flags & extra->flags) == 0)
		return KEVENT_CB_NONE;

	extra->task->state = TASK_QUEUE;
	kfree(extra);
	file_put(extra->file);
	kent_put(&extra->task->kent);
	return KEVENT_CB_LISTENER_DEL | KEVENT_CB_STOP;
}

static int iowait_device(struct file *file, enum device_kevent_flags flags)
{
	file_get(file->fd);
	kent_get(&current->kent);

	/* this must be atomic because event listeners can't sleep but need to call free() */
	struct io_device_kevent_extra *extra = kmalloc(sizeof(*extra), MEM_KERNEL | MEM_ATOMIC);
	if (extra == NULL)
		return -ENOMEM;

	extra->file = file;
	extra->task = current;
	extra->flags = flags;

	kevent_listener_add(KEVENT_DEVICE, io_device_kevent_listener, extra);
	yield(TASK_IOWAIT);
	return 0;
}

ssize_t file_read(void *buf, struct file *file, size_t len)
{
	if (len == 0)
		return 0;

	ssize_t ret = 0;

	mutex_lock(&file->lock);

	while (ret < (ssize_t)len) {
		ssize_t tmp = file->device->read(buf, file->device, len, file->pos);
		if (tmp < 0) {
			if (tmp == -EBUSY) {
				tmp = iowait_device(file, DEVICE_KEVENT_RX);
			} else {
				ret = tmp;
				break;
			}
		}

		if (file->type == FILE_TYPE_REGULAR)
			file->pos += tmp;

		ret += tmp;
		buf += tmp;
	}

	mutex_unlock(&file->lock);
	file_kevent_create_and_dispatch(file, FILE_KEVENT_READ);

	return ret;
}

ssize_t file_write(struct file *file, const void *buf, size_t len)
{
	if (len == 0)
		return 0;

	ssize_t ret = 0;

	mutex_lock(&file->lock);

	while (ret < (ssize_t)len) {
		ssize_t tmp = file->device->write(file->device, buf, len, file->pos);
		if (tmp < 0) {
			if (tmp == -EBUSY) {
				tmp = iowait_device(file, DEVICE_KEVENT_TX);
				if (tmp < 0) {
					ret = tmp;
					break;
				}
			} else {
				ret = tmp;
				break;
			}
		}

		if (file->type == FILE_TYPE_REGULAR)
			file->pos += tmp;

		ret += tmp;
		buf += tmp;
	}

	mutex_unlock(&file->lock);
	file_kevent_create_and_dispatch(file, FILE_KEVENT_WRITE );

	return ret;
}

static void file_kevent_destroy(struct kent *kent)
{
	struct kevent *kevent = container_of(kent, struct kevent, kent);
	struct file_kevent *file_kevent = container_of(kevent, struct file_kevent, kevent);
	kfree(file_kevent);
}

struct file_kevent *file_kevent_create(struct file *f, enum file_kevent_flags flags)
{
	struct file_kevent *event = kmalloc(sizeof(*event), MEM_KERNEL | MEM_ATOMIC);
	if (event == NULL)
		return NULL;

	event->flags = flags;
	event->kevent.kind = KEVENT_FILE;

	event->kevent.kent.parent = &f->kent;
	event->kevent.kent.destroy = file_kevent_destroy;
	int err = kent_init(&event->kevent.kent);
	if (err != 0) {
		kfree(event);
		event = NULL;
	}

	return event;
}

void file_kevent_create_and_dispatch(struct file *f, enum file_kevent_flags flags)
{
	struct file_kevent *event = file_kevent_create(f, flags);
	if (event != NULL)
		kevent_dispatch(&event->kevent);
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
