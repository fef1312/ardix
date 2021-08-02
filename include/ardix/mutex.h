/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/types.h>

#include <errno.h>
#include <toolchain.h>

/**
 * @defgroup mutex Synchronization Primitives
 *
 * @{
 */

/**
 * @brief A simple mutex.
 *
 * Mutexes can be locked using the `mutex_lock()` and `mutex_unlock()` methods
 * respectively.  The former will block until the lock is acquired and thus
 * should never be used from interrupt context.  Use `mutex_trylock()` if you
 * don't want blocking.
 */
struct mutex {
	uint8_t lock;	/**< Current lock value, don't read directly */
};

/**
 * @brief Internal assembly routine for `mutex_lock()`.
 * @private
 */
extern void _mutex_lock(uint8_t *lock);
/**
 * @brief Internal assembly routine for `mutex_trylock()`.
 * @private
 */
extern int _mutex_trylock(uint8_t *lock);
/**
 * @brief Internal assembly routine for `mutex_unlock()`.
 * @private
 */
extern void _mutex_unlock(uint8_t *lock);

/**
 * @brief Initialize a mutex and set it to unlocked.
 *
 * @param mutex Mutex to initialize
 */
__always_inline void mutex_init(struct mutex *mutex)
{
	mutex->lock = 0;
}

/**
 * @brief Acquire an exclusive lock on a mutex.
 * This call will block until the lock was acquired and therefore cannot fail.
 *
 * @param mutex Mutex to lock
 */
__always_inline void mutex_lock(struct mutex *mutex)
{
	_mutex_lock(&mutex->lock);
}

/**
 * @brief Release an exclusive lock on a mutex.
 *
 * @param mutex Mutex to unlock
 */
__always_inline void mutex_unlock(struct mutex *mutex)
{
	_mutex_unlock(&mutex->lock);
}

/**
 * @brief Attempt to acquire an exclusive lock on a mutex.
 * The return value is zero if the claim succeeds.
 *
 * @param mutex Mutex to attempt to lock
 * @returns 0 if the lock was acquired, `-EAGAIN` otherwise
 */
__always_inline int mutex_trylock(struct mutex *mutex)
{
	if (_mutex_trylock(&mutex->lock) == 0)
		return 0;
	else
		return -EAGAIN;
}

/**
 * @brief Determine whether a mutex is locked.
 *
 * @param mutex Mutex to get the lock value of
 * @returns Nonzero if the mutex is locked, zero otherwise
 */
__always_inline int mutex_is_locked(struct mutex *mutex)
{
	return mutex->lock;
}

/**
 * @brief Statically declare and define a mutex (file scope only).
 * The mutex does not need to be initialized using `mutex_init()`.
 *
 * @param name Name of the `struct mutex` that will be defined
 */
#define MUTEX(name) struct mutex name = { .lock = 0 }

/** @} */

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
