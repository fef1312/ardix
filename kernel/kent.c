/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/atom.h>
#include <ardix/malloc.h>
#include <ardix/kent.h>
#include <ardix/list.h>

#include <errno.h>
#include <stddef.h>

struct kent _kent_root;
struct kent *kent_root = NULL;

int kent_root_init(void)
{
	if (kent_root != NULL)
		return -EEXIST;
	kent_root = &_kent_root;

	kent_root->parent = NULL;
	kent_root->destroy = NULL;
	atom_init(&kent_root->refcount);
	kent_get(kent_root);

	return 0;
}

int kent_init(struct kent *kent)
{
	if (kent->parent == NULL || kent->destroy == NULL)
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
		kent->destroy(kent);

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
