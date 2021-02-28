/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/sched.h>
#include <arch/watchdog.h>

#include <ardix/atomic.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/string.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>

extern uint32_t _sstack;
extern uint32_t _estack;

static struct task *_sched_tasktab[CONFIG_SCHED_MAXTASK];
struct task *_sched_current_task;

int sched_init(void)
{
	int i;

	_sched_current_task = malloc(sizeof(*_sched_current_task));
	if (_sched_current_task == NULL)
		return -ENOMEM;

	_sched_current_task->sp = &_sstack;
	_sched_current_task->stack_bottom = &_estack;
	_sched_current_task->pid = 0;
	_sched_current_task->state = TASK_READY;
	_sched_tasktab[0] = _sched_current_task;

	for (i = 1; i < CONFIG_SCHED_MAXTASK; i++)
		_sched_tasktab[i] = NULL;

	i = arch_watchdog_init();

	if (i == 0)
		i = arch_sched_hwtimer_init(CONFIG_SCHED_MAXTASK);

	return i;
}

/**
 * Determine whether the specified task is a candidate for execution.
 *
 * @param task: the task
 * @returns whether `task` could be run next
 */
static inline bool sched_task_should_run(const struct task *task)
{
	enum task_state state = task->state;

	if (state == TASK_QUEUE || state == TASK_READY || state == TASK_IOWAIT)
		return true;

	return false;
}

void *sched_process_switch(void *curr_sp)
{
	struct task *tmp;
	pid_t nextpid = _sched_current_task->pid;
	_sched_current_task->sp = curr_sp;

	if (_sched_current_task->state != TASK_SLEEP && _sched_current_task->state != TASK_IOWAIT)
		_sched_current_task->state = TASK_QUEUE;

	while (1) {
		nextpid++;
		nextpid %= CONFIG_SCHED_MAXTASK;
		tmp = _sched_tasktab[nextpid];
		if (tmp != NULL && sched_task_should_run(tmp)) {
			_sched_current_task = tmp;
			break;
		}
		/* TODO: Add idle thread */
	}

	_sched_current_task->state = TASK_READY;
	return _sched_current_task->sp;
}

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
