/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>
#include <toolchain.h>

__shared ssize_t read(int fildes, void *buf, size_t nbyte);
__shared ssize_t write(int fildes, const void *buf, size_t nbyte);
__shared ssize_t sleep(unsigned long int millis);
/**
 * @brief Create a new thread.
 *
 * Embedded systems typically don't have a MMU and thus no virtual memory,
 * meaning it is impossible to implement a proper fork.  So, the `fork()` and
 * `execve()` system calls have to be combined into one.
 */
__shared pid_t exec(int (*entry)(void));
__shared __noreturn void exit(int status);
__shared pid_t waitpid(pid_t pid, int *stat_loc, int options);

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
