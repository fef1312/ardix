/* See the end of this file for copyright, license, and warranty information. */

/* Simple ring buffer implementation. */

#include <ardix/malloc.h>
#include <ardix/ringbuf.h>
#include <ardix/types.h>

#include <stddef.h>

struct ringbuf *ringbuf_create(size_t size)
{
	struct ringbuf *buf = kmalloc(sizeof(*buf) + size);
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
	kfree(buf);
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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
