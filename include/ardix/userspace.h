/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>

#include <toolchain.h>

/**
 * Copy data from user space to kernel space.
 *
 * @param dest: where to copy to
 * @param src: where to copy from
 * @param len: amount of bytes to copy
 * @returns amount of bytes copied
 */
size_t copy_from_user(void *dest, __user const void *src, size_t len);

/**
 * Copy data from kernel space to user space.
 *
 * @param dest: where to copy to
 * @param src: where to copy from
 * @param len: amount of bytes to copy
 * @returns amount of bytes copied
 */
size_t copy_to_user(__user void *dest, __user const void *src, size_t len);

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
