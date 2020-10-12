/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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
	(head).next = &(head);		\
	(head).prev = &(head);		\
})

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_next_entry(ptr, member) \
	container_of((ptr)->member->next, typeof(*(ptr)), member)

#define list_prev_entry(ptr, member) \
	container_of((ptr)->member->prev, typeof(*(ptr)), member)

#define list_first_entry(head, type, member) \
	container_of((head)->next, type, member)

#define list_last_entry(head, type, memner) \
	container_of((head)->prev, type, member)

#define list_is_empty(head) ((head)->next == head)

#define list_for_each(head, cursor)		\
	for (cursor = (head)->next;		\
	     cursor != head;			\
	     cursor = (cursor)->next)

#define list_for_each_reverse(head, cursor)	\
	for (cursor = (head)->prev;		\
	     cursor != head;			\
	     cursor = (cursor)->prev)

#define list_for_each_safe(head, cursor, tmp)		\
	for (cursor = (head)->next, tmp = cursor;	\
	     tmp != head;				\
	     tmp = (tmp)->next, cursor = tmp)

#define list_for_each_entry(head, cursor, member)				\
	for (cursor = list_first_entry(head, typeof(*(cursor)), member);	\
	     &(cursor)->member != head;						\
	     cursor = list_next_entry(cursor, member))

#define list_for_each_entry_reverse(head, cursor, member)			\
	for (cursor = list_last_entry(head, typeof(*(cursor)), member);		\
	     &(cursor)->member != head;						\
	     cursor = list_prev_entry(cursor, member))

#define list_for_each_entry_safe(head, cursor, tmp, member)			\
	for (cursor = list_first_entry(head, typeof(*(cursor)), member),	\
	     tmp = list_next_entry(cursor, member);				\
	     tmp != head;							\
	     cursor = tmp,							\
	     tmp = list_next_entry(tmp, member))

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
