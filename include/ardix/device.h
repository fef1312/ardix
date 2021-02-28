/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

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
#define kent_to_device(ptr) container_of(ptr, struct device, kent)

extern struct kent *devices_kent;

/** Initialize the devices subsystem. */
int devices_init(void);

/**
 * Initialize a device and add it to the device tree.
 *
 * @param dev: device to initialze
 * @returns 0 on success, or a negative error code on failure
 */
int device_init(struct device *dev);

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
