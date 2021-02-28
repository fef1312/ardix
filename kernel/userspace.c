/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/types.h>
#include <ardix/userspace.h>

#include <ardix/string.h>
#include <toolchain.h>

/*
 * These don't do anything special because there is no MPU or other protection
 * yet, but having them as a wrapper this early is probably a good idea because
 * it make life easier when the MPU is active.
 */

size_t copy_from_user(void *dest, __user const void *src, size_t len)
{
	memcpy(dest, src, len);
	return len;
}

size_t copy_to_user(__user void *dest, const void *src, size_t len)
{
	memcpy(dest, src, len);
	return len;
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
