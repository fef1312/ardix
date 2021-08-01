/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdarg.h>
#include <toolchain.h>

/*
 * We sacrifice performance for less memory usage by not having a userland
 * structure for `FILE`.  Instead, we cast the file descriptor to a pointer and
 * use that for syscalls.
 */
struct _user_file { void *_undefined; };
#define FILE struct _user_file

#define _file_to_fd(f) ((int)( (intptr_t)(f) ))
#define _fd_to_file(fd) ((FILE *)( (intptr_t)(fd) ))

#define stdin _fd_to_file(0)
#define stdout _fd_to_file(1)
#define stderr _fd_to_file(2)

__shared int printf(const char *restrict fmt, ...)
__attribute__(( format(printf, 1, 2) ));

__shared int vprintf(const char *restrict fmt, va_list args);

__shared int fprintf(FILE *f, const char *restrict fmt, ...)
__attribute__(( format(printf, 2, 3) ));

__shared int vfprintf(FILE *f, const char *restrict fmt, va_list args);

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
