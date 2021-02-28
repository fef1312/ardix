/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

/* Simple ring buffer implementation. */

#include <ardix/malloc.h>
#include <ardix/ringbuf.h>
#include <ardix/types.h>
#include <stddef.h>

struct ringbuf *ringbuf_create(size_t size)
{
	struct ringbuf *buf = malloc(sizeof(*buf) + size);
	if (buf == NULL)
		return NULL;

	buf->capacity = size;
	buf->len = 0;
	buf->rpos = 0;
	buf->wpos = 0;

	return buf;
}

inline void ringbuf_destroy(struct ringbuf *buf)
{
	free(buf);
}

size_t ringbuf_read(void *dest, struct ringbuf *buf, size_t len)
{
	uint8_t *tmp = dest;

	while (len-- && buf->len) {
		*tmp++ = buf->data[buf->rpos++];
		buf->len--;

		/* wrap around if required */
		buf->rpos %= buf->capacity;
	}

	return (size_t)tmp - (size_t)dest;
}

size_t ringbuf_write(struct ringbuf *buf, const void *src, size_t len)
{
	const uint8_t *tmp = src;

	while (len-- && buf->len != buf->capacity) {
		buf->data[buf->wpos++] = *tmp++;
		buf->len++;

		/* wrap around if required */
		buf->wpos %= buf->capacity;
	}

	return (size_t)tmp - (size_t)src;
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
