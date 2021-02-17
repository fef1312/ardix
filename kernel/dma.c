/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/device.h>
#include <ardix/dma.h>
#include <ardix/kent.h>
#include <ardix/malloc.h>
#include <ardix/types.h>
#include <ardix/util.h>

#include <stddef.h>

static void dmabuf_destroy(struct kent *kent)
{
	struct dmabuf *buf = container_of(kent, struct dmabuf, kent);
	free(buf);
}

static struct kent_ops dma_kent_ops = {
	.destroy = &dmabuf_destroy,
};

struct dmabuf *dmabuf_create(struct device *dev, size_t len)
{
	int err = 0;
	struct dmabuf *buf = malloc(sizeof(*buf) + len);
	if (buf == NULL)
		return NULL;

	buf->kent.operations = &dma_kent_ops;

	err = kent_init(&buf->kent, &dev->kent);
	if (err != 0) {
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
 * Copyright (c) 2021 Felix Kopp <sandtler@sandtler.club>
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
