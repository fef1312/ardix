/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>

#if CONFIG_SCHED_MAXPROC < 128
#define _PID_TYPE_ int8_t
#elif CONFIG_SCHED_MAXPROC < 32768
#define _PID_TYPE_ int16_t
#else
#define _PID_TYPE_ int32_t
#endif /* SCHED_MAXPROC */

/** Process identifier. */
typedef _PID_TYPE_		pid_t;

/** Simple atomic reference counter */
typedef struct {
	int count;
} atom_t;

#include <arch/hardware.h>

/* Syscall argument */
typedef word_t	sysarg_t;

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
