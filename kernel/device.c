/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/device.h>
#include <ardix/kent.h>
#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>

struct kent *devices_kent = NULL;

static void device_destroy(struct kent *kent)
{
	struct device *dev = to_device(kent);
	free(dev);
}

struct kent_ops devices_kent_ops = {
	.destroy = &device_destroy,
};

/** Initialize the devices subsystem. */
int devices_init(void)
{
	if (devices_kent != NULL)
		return -EEXIST;

	devices_kent = malloc(sizeof(*devices_kent));
	if (devices_kent == NULL)
		return -ENOMEM;

	/* we don't need that because the root device kent lives forever */
	devices_kent->operations = NULL;

	return kent_init(devices_kent, NULL);
}

int device_init(struct device *dev, struct device *parent)
{
	if (devices_kent == NULL)
		return -ENOENT;

	return kent_init(&dev->kent, devices_kent);
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
