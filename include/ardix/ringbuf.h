/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>

struct ringbuf {
	/** Total capacity */
	size_t capacity;
	/** Amount of bytes currently available for reading */
	size_t len;
	size_t rpos;
	size_t wpos;
	uint8_t data[];
};

/**
 * Create and initialize a new ring buffer.
 *
 * @param size: Total buffer size in bytes.
 * @returns The newly allocated buffer, or `NULL` if OOM.
 */
struct ringbuf *ringbuf_create(size_t size);

/**
 * Destroy a ring buffer previously created with `ringbuf_create`.
 *
 * @param buf: The buffer to destroy.
 */
void ringbuf_destroy(struct ringbuf *buf);

/**
 * Read up to `len` bytes from the buffer.
 * Any byte that has been read once is discarded from the buffer.
 * If there are less than `len` bytes stored in the buffer, reading stops early.
 *
 * @param dest: Where to write the data to.
 * @param buf: The buffer to read from.
 * @param len: The maximum amount of bytes to read.
 * @returns The actual amount of bytes read.
 */
size_t ringbuf_read(void *dest, struct ringbuf *buf, size_t len);

/**
 * Write up to `len` bytes to the buffer.
 * If the buffer would overflow, writing stops early.
 *
 * @param buf: The buffer to write to.
 * @param src: The data to write.
 * @param len: The length of `src`.
 * @returns The actual amount of bytes that were written.
 */
size_t ringbuf_write(struct ringbuf *buf, const void *src, size_t len);

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
