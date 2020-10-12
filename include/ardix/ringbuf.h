/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <ardix/types.h>

struct ringbuf {
	size_t size;
	uint8_t *rpos;
	uint8_t *wpos;
	uint8_t data[];
};

/**
 * Create and initialize a new ring buffer.
 *
 * @param size Total buffer size in bytes.
 * @returns The newly allocated buffer, or `NULL` if OOM.
 */
struct ringbuf *rungbuf_create(size_t size);

/**
 * Destroy a ring buffer previously created with `ringbuf_create`.
 *
 * @param buf The buffer to destroy.
 */
void ringbuf_destroy(struct ringbuf *buf);

/**
 * Read up to `len` bytes from the buffer.
 * Any byte that has been read once is discarded from the buffer.
 * If there are less than `len` bytes stored in the buffer, reading stops early.
 *
 * @param dest Where to write the data to.
 * @param buf The buffer to read from.
 * @param len The maximum amount of bytes to read.
 * @returns The actual amount of bytes read.
 */
size_t ringbuf_read(uint8_t *dest, struct ringbuf *buf, size_t len);

/**
 * Write up to `len` bytes to the buffer.
 * If the buffer would overflow, writing stops early.
 *
 * @param buf The buffer to write to.
 * @param src The data to write.
 * @param len The length of `src`.
 * @returns The actual amount of bytes that were written.
 */
size_t ringbuf_write(struct ringbuf *buf, const uint8_t *src, size_t len);

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
