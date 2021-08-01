/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <toolchain.h>

struct mutex {
	int lock;
};

extern int _mutex_lock(int *lock);
extern int _mutex_trylock(int *lock);
extern void _mutex_unlock(int *lock);

__always_inline void mutex_init(struct mutex *mutex)
{
	mutex->lock = 0;
}

__always_inline int mutex_lock(struct mutex *mutex)
{
	return _mutex_lock(&mutex->lock);
}

__always_inline void mutex_unlock(struct mutex *mutex)
{
	_mutex_unlock(&mutex->lock);
}

__always_inline int mutex_trylock(struct mutex *mutex)
{
	return _mutex_trylock(&mutex->lock);
}

#define MUTEX(name) struct mutex name = { .lock = 0 }

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
