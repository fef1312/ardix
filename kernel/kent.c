/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/atom.h>
#include <ardix/malloc.h>
#include <ardix/kent.h>
#include <ardix/list.h>

#include <errno.h>
#include <stddef.h>

struct kent *kent_root = NULL;

static void kent_root_destroy(struct kent *kent)
{
	/*
	 * this callback should never actually be executed in the first place
	 * because the kent root lives as long as the kernel is running but hey,
	 * it's not like our flash memory has a size limit or anything :)
	 */
	free(kent);
	kent_root = NULL;
}

static struct kent_ops kent_root_ops = {
	.destroy = &kent_root_destroy,
};

int kent_root_init(void)
{
	if (kent_root != NULL)
		return -EEXIST;

	kent_root = malloc(sizeof(*kent_root));
	if (kent_root == NULL)
		return -ENOMEM;

	kent_root->parent = NULL;
	kent_root->operations = &kent_root_ops;
	atom_init(&kent_root->refcount);
	kent_get(kent_root);

	return 0;
}

int kent_init(struct kent *kent)
{
	if (kent->parent == NULL || kent->operations == NULL)
		return -EFAULT;
	kent_get(kent->parent);

	atom_init(&kent->refcount);
	kent_get(kent);

	return 0;
}

void kent_get(struct kent *kent)
{
	atom_get(&kent->refcount);
}

void kent_put(struct kent *kent)
{
	struct kent *parent = kent->parent;

	if (atom_put(&kent->refcount) == 0) {
		kent->operations->destroy(kent);

		if (parent != NULL)
			kent_put(parent);
	}
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
