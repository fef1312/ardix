/* SPDX-License-Identifier: GPL-3.0-or-later */
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
	free(buf);
}

static struct kent_ops dmabuf_kent_ops = {
	.destroy = &dmabuf_destroy,
};

struct dmabuf *dmabuf_create(struct device *dev, size_t len)
{
	int err = 0;
	struct dmabuf *buf = malloc(sizeof(*buf) + len);
	if (buf == NULL)
		return NULL;

	buf->kent.parent = &dev->kent;
	buf->kent.operations = &dmabuf_kent_ops;

	err = kent_init(&buf->kent);
	if (err) {
		free(buf);
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
