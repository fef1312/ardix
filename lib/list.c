/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/list.h>

void list_insert(struct list_head *head, struct list_head *new)
{
	new->next = head->next;
	head->next->prev = new;

	new->prev = head;
	head->next = new;
}

void list_insert_before(struct list_head *head, struct list_head *new)
{
	new->next = head;
	head->prev->next = new;

	new->prev = head->prev;
	head->prev = new;
}

void list_delete(struct list_head *head)
{
	head->next->prev = head->prev;
	head->prev->next = head->next;
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
