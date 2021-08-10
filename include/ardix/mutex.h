/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/atomic.h>
#include <ardix/list.h>
#include <ardix/types.h>

#include <errno.h>
#include <toolchain.h>

/**
 * @defgroup mutex Synchronization Primitives
 *
 * @{
 */

/**
 * @brief A spinning mutex.
 * This is the most primitive type of mutex.  It is locked by looping (spinning)
 * so long until the lock could be acquired, thus blocking the CPU from doing
 * anything else.  For this reason, tasks that hold a spinlock must not sleep
 * while holding such a lock.  This behavior is enforced by calling
 * `atomic_enter()` in the `spin_lock()` function and `atomic_leave()` in
 * `spin_unlock()`.
 */
typedef struct {
	uint8_t lock;
} spin_t;

extern void _spin_lock(uint8_t *lock);
extern void _spin_unlock(uint8_t *lock);
extern int _spin_trylock(uint8_t *lock);

__always_inline void spin_init(spin_t *spin)
{
	spin->lock = 0;
}

__always_inline void spin_lock(spin_t *spin)
{
	atomic_enter();
	_spin_lock(&spin->lock);
}

__always_inline void spin_unlock(spin_t *spin)
{
	_spin_unlock(&spin->lock);
	atomic_leave();
}

__always_inline int spin_trylock(spin_t *spin)
{
	if (_spin_trylock(&spin->lock) == 0)
		return 0;
	else
		return -EAGAIN;
}

__always_inline bool spin_is_locked(spin_t *spin)
{
	return spin->lock != 0;
}

/**
 * @brief A simple mutex.
 *
 * Mutexes can be locked and unlocked using the `mutex_lock()` and
 * `mutex_unlock()` methods respectively.  The former will block until the lock
 * is acquired and thus should never be used from interrupt context.
 * Use `mutex_trylock()` if you don't want blocking.
 */
struct mutex {
	uint8_t lock;	/**< Current lock value, don't read directly */
	spin_t wait_queue_lock;
	struct list_head wait_queue; /**< -> mutex_wait::link */
};

struct mutex_wait {
	struct list_head link;
	struct task *task;
};

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
void mutex_init(struct mutex *mutex);

/**
 * @brief Acquire an exclusive lock on a mutex.
 * This call may sleep if the lock cannot be acquired instantly.
 *
 * @param mutex Mutex to lock
 */
void mutex_lock(struct mutex *mutex);

/**
 * @brief Release an exclusive lock on a mutex.
 *
 * @param mutex Mutex to unlock
 */
void mutex_unlock(struct mutex *mutex);

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
 * @returns true if the mutex is locked
 */
__always_inline bool mutex_is_locked(struct mutex *mutex)
{
	return mutex->lock != 0;
}

/**
 * @brief Statically declare and define a mutex (file scope only).
 * The mutex does not need to be initialized using `mutex_init()`.
 *
 * @param name Name of the `struct mutex` that will be defined
 */
#define MUTEX(name) struct mutex name = {		\
	.lock = 0,					\
	.wait_queue_lock = { .lock = 0 },		\
	.wait_queue = LIST_HEAD_INIT(name.wait_queue)	\
}

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
