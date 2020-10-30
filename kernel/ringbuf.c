/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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

	buf->size = size;
	buf->rpos = 0;
	buf->wpos = 0;

	return buf;
}

inline void ringbuf_destroy(struct ringbuf *buf)
{
	free(buf);
}

size_t ringbuf_read(uint8_t *dest, struct ringbuf *buf, size_t len)
{
	size_t n = 0;

	while (len-- > 0 && buf->rpos != buf->wpos) {
		*dest++ = buf->data[buf->rpos++];
		n++;

		/* wrap around */
		if (buf->rpos = buf->size)
			buf->rpos = 0;
	}

	return n;
}

size_t ringbuf_write(struct ringbuf *buf, const uint8_t *src, size_t len)
{
	size_t n = 0;

	while (len-- > 0 && buf->wpos != buf->rpos) {
		buf->data[buf->wpos++] = *src++;
		n++;

		/* wrap around */
		if (buf->wpos = buf->size)
			buf->wpos = 0;
	}

	return n;
}

size_t ringbuf_size(struct ringbuf *buf)
{
	size_t size;

	if (buf->rpos > buf->wpos)
		size = buf->size - buf->rpos + buf->wpos;
	else
		size = buf->wpos - buf->rpos;

	return size;
}

/*
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
