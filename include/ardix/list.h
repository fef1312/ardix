/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/util.h>

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

#define LIST_HEAD(name)			\
	struct list_head name = {	\
		.next = &name,		\
		.prev = &name,		\
	}

#define list_init(head) ({		\
	(head)->next = head;		\
	(head)->prev = head;		\
})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_next_entry(ptr, member) \
	container_of((ptr)->member.next, typeof(*(ptr)), member)

#define list_prev_entry(ptr, member) \
	container_of((ptr)->member.prev, typeof(*(ptr)), member)

#define list_first_entry(head, type, member) \
	container_of((head)->next, type, member)

#define list_last_entry(head, type, member) \
	container_of((head)->prev, type, member)

#define list_is_empty(head) ((head)->next == (head))

#define list_for_each(head, cursor)		\
	for (cursor = (head)->next;		\
	     cursor != (head);			\
	     cursor = (cursor)->next)

#define list_for_each_reverse(head, cursor)	\
	for (cursor = (head)->prev;		\
	     cursor != (head);			\
	     cursor = (cursor)->prev)

#define list_for_each_safe(head, cursor, tmp)		\
	for (cursor = (head)->next, tmp = cursor;	\
	     tmp != (head);				\
	     tmp = (tmp)->next, cursor = tmp)

#define list_for_each_entry(head, cursor, member)				\
	for (cursor = list_first_entry(head, typeof(*(cursor)), member);	\
	     &(cursor)->member != (head);					\
	     cursor = list_next_entry(cursor, member))

#define list_for_each_entry_reverse(head, cursor, member)			\
	for (cursor = list_last_entry(head, typeof(*(cursor)), member);		\
	     &(cursor)->member != (head);					\
	     cursor = list_prev_entry(cursor, member))

#define list_for_each_entry_safe(head, cursor, tmp, member)			\
	for (cursor = list_first_entry(head, typeof(*(cursor)), member),	\
		 tmp = list_next_entry(cursor, member);				\
	     tmp != (head);							\
	     cursor = tmp, tmp = list_next_entry(tmp, member))

void list_insert(struct list_head *head, struct list_head *new);

void list_insert_before(struct list_head *head, struct list_head *new);

void list_delete(struct list_head *head);

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
