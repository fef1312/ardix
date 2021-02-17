/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

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
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
