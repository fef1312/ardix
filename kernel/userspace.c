/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/types.h>
#include <ardix/userspace.h>

#include <string.h>
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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
