/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/**
 * Cast a pointer to a member of a struct out to the containing structure.
 *
 * @param ptr A pointer to the nested struct.
 * @param type The type of the containing struct.
 * @param member Name of the member within the containing struct.
 * @return A pointer to the containing struct.
 */
#define container_of(ptr, type, member) \
	( (type *)((void *)(ptr) - offsetof(type, member)) )

/**
 * Get the size of a struct member.
 *
 * @param type The type of the containing struct.
 * @param member The name of the member within the struct.
 * @return The size of the member in bytes.
 */
#define SIZEOF_MEMBER(type, member) \
	(sizeof( ((type *)0)->member ))

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
