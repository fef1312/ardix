/* SPDX-License-Identifier: GPL-3.0-or-later */
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
