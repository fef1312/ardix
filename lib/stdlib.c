/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/syscall.h>

#include <stddef.h>
#include <stdlib.h>

/*
 * kmalloc() and free() are system calls in Ardix because the heap is shared
 * among all tasks and locked using a mutex.  If the lock is already claimed,
 * the `mutex_lock()` routine will suspend the current task until the lock
 * becomes available to the current process.  However, this can only happen
 * when we already are in kernel space.
 */

void *malloc(size_t size)
{
	if (size == 0) {
		return NULL;
	} else {
		long int intptr = syscall(SYS_malloc, (sysarg_t)size);
		return *(void **)&intptr;
	}
}

void *calloc(size_t nmemb, size_t size)
{
	size_t total = nmemb * size;
	if (nmemb != 0 && total / nmemb != size)
		return NULL; /* overflow check as mandated by POSIX.1 */
	long int intptr = syscall(SYS_malloc, (sysarg_t)total);
	return *(void **)&intptr;
}

void free(void *ptr)
{
	if (ptr != NULL)
		syscall(SYS_free, (sysarg_t)ptr);
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
