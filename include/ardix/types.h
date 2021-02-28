/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>

#if CONFIG_SCHED_MAXPROC < 128
#define _PID_TYPE_ int8_t
#elif CONFIG_SCHED_MAXPROC < 32767
#define _PID_TYPE_ int16_t
#else
#define _PID_TYPE_ int32_t
#endif /* SCHED_MAXPROC */

/** Process identifier. */
typedef _PID_TYPE_		pid_t;

#ifndef __SIG_ATOMIC_TYPE__
#define __SIG_ATOMIC_TYPE__ int
#endif /* __SIG_ATOMIC_TYPE__ */

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
 * Ardix is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ardix is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
