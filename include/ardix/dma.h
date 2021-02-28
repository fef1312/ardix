/* SPDX-License-Identifier: GPL-3.0-or-later */
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
