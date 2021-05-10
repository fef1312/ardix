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
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
