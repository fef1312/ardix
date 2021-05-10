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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
