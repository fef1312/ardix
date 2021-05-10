/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/device.h>
#include <ardix/kent.h>
#include <ardix/types.h>

struct dmabuf {
	struct kent kent;
	size_t len;
	uint8_t data[0];
};

#define kent_to_dmabuf(ptr) container_of(ptr, struct dmabuf, kent)

/**
 * Create a new DMA buffer and its corresponding kent.
 * Use `dmabuf_get` and `dmabuf_put` for refcounting.
 *
 * @param dev: device to create the buffer for
 * @param len: buffer length in bytes
 * @returns a pointer to the buffer, or `NULL` on failure
 */
struct dmabuf *dmabuf_create(struct device *dev, size_t len);

/** Increment a DMA buffer's reference counter. */
void dmabuf_get(struct dmabuf *buf);

/** Decrement a DMA buffer's reference counter. */
void dmabuf_put(struct dmabuf *buf);

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
