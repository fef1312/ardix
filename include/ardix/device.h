/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <ardix/kent.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/types.h>

#include <stddef.h>

/** Top-level abstraction for any device connected to the system. */
struct device {
	struct kent kent;
};

/** Cast a kent out to its containing struct device */
#define to_device(ptr) container_of(ptr, struct device, kent)

extern struct kent *devices_kent;

/** Initialize the devices subsystem. */
int devices_init(void);

/**
 * Initialize a device and add it to the device tree.
 *
 * @param dev: device to initialze
 * @param parent: parent device (may me `NULL` if unapplicable)
 * @returns 0 on success, or a negative error code on failure
 */
int device_init(struct device *dev, struct device *parent);

/** Increment a device's reference counter. */
inline void device_get(struct device *dev)
{
	kent_get(&dev->kent);
}

/** Decrement a device's referece counter. */
inline void device_put(struct device *dev)
{
	kent_put(&dev->kent);
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
