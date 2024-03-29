/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/device.h>
#include <ardix/dma.h>
#include <ardix/kent.h>
#include <ardix/malloc.h>
#include <ardix/types.h>
#include <ardix/util.h>

#include <stddef.h>

static void dmabuf_destroy(struct kent *kent)
{
	struct dmabuf *buf = kent_to_dmabuf(kent);
	kfree(buf);
}

struct dmabuf *dmabuf_create(struct device *dev, size_t len)
{
	int err = 0;
	/*
	 * allocation needs to be atomic because the buffer might be
	 * free()d from within an irq handler which cannot sleep
	 */
	struct dmabuf *buf = atomic_kmalloc(sizeof(*buf) + len);
	if (buf == NULL)
		return NULL;

	buf->kent.parent = &dev->kent;
	buf->kent.destroy = dmabuf_destroy;

	err = kent_init(&buf->kent);
	if (err) {
		kfree(buf);
		return NULL;
	}

	buf->len = len;

	return buf;
}

void dmabuf_get(struct dmabuf *buf)
{
	kent_get(&buf->kent);
}

void dmabuf_put(struct dmabuf *buf)
{
	kent_put(&buf->kent);
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
