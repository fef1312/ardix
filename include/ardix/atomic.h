/* See the end of this file for copyright, license, and warranty information. */

#pragma once

/**
 * Enter atomic context.
 *
 * Since Ardix does not support SMP, this method will simply increment a
 * reference counter that is checked in the scheduler interrupt routine before
 * performing the context switch.
 */
void atomic_enter(void);

/** Leave atomic context. */
void atomic_leave(void);

/** Return a nonzero value if the current process is in atomic context. */
int is_atomic_context(void);

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
