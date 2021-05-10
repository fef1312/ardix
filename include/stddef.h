/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>

#ifndef offsetof
/**
 * Calculate the byte offset of a struct member relative to the struct itself.
 *
 * @param type: The structure type.
 * @param member: The member inside the struct to the offset of.
 * @returns The offset of `member` reelative to `type`, casted to a `size_t`.
 */
#define offsetof(type, member) ((size_t)&((type *)0)->member)
#endif /* offsetof */

#ifndef NULL
/** The `NULL` pointer. */
#define NULL ((void *)0)
#endif /* NULL */

#include <stdbool.h>

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
