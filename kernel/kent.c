/* See the end of this file for copyright, license, and warranty information. */

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
