/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <arch/hardware.h>
#include <arch/sched.h>

#include <ardix/list.h>
#include <ardix/malloc.h>
#include <ardix/sched.h>
#include <ardix/string.h>
#include <ardix/types.h>

#include <errno.h>
#include <stddef.h>

extern uint32_t _sstack;
extern uint32_t _estack;

struct process *proc_table[CONFIG_SCHED_MAXPROC];
struct process *_current_process;

bool _is_atomic_context = false;

int sched_init(void)
{
	int i;

	_current_process = malloc(sizeof(*_current_process));
	if (_current_process == NULL)
		return -ENOMEM;

	_current_process->sp = &_sstack;
	_current_process->stack_bottom = &_estack;
	_current_process->pid = 0;
	_current_process->state = PROC_READY;
	proc_table[0] = _current_process;

	for (i = 1; i < CONFIG_SCHED_MAXPROC; i++)
		proc_table[i] = NULL;

	return arch_sched_hwtimer_init(CONFIG_SCHED_INTR_FREQ);
}

/**
 * Determine whether the specified process should be executed next.
 *
 * @param proc: The process.
 * @returns Whether `proc` should be next in line for the scheduler.
 */
static inline bool sched_proc_should_run(const struct process *proc)
{
	enum proc_state state = proc->state;

	if (state == PROC_QUEUE || state == PROC_READY || state == PROC_IOWAIT)
		return true;

	return false;
}

void *sched_process_switch(void *curr_sp)
{
	pid_t nextpid = _current_process->pid;
	_current_process->sp = curr_sp;

	if (_current_process->state != PROC_SLEEP && _current_process->state != PROC_IOWAIT)
		_current_process->state = PROC_QUEUE;

	while (1) {
		nextpid++;
		nextpid %= CONFIG_SCHED_MAXPROC;
		if (proc_table[nextpid] != NULL && sched_proc_should_run(proc_table[nextpid])) {
			_current_process = proc_table[nextpid];
			break;
		}
		/* TODO: Add idle thread */
	}

	_current_process->state = PROC_READY;
	return _current_process->sp;
}

struct process *sched_process_create(void (*entry)(void))
{
	pid_t pid;
	struct process *proc = malloc(sizeof(*proc));
	if (proc == NULL)
		return NULL;

	sched_atomic_enter();

	for (pid = 1; pid < CONFIG_SCHED_MAXPROC; pid++) {
		if (proc_table[pid] == NULL)
			break;
	}

	if (pid == CONFIG_SCHED_MAXPROC) {
		/* max number of processess exceeded */
		free(proc);
		sched_atomic_leave();
		return NULL;
	}

	proc->pid = pid;
	proc->stack_bottom = &_estack - (pid * (signed)CONFIG_STACKSZ);
	proc->lastexec = 0;
	proc->sleep_usecs = 0;
	proc->state = PROC_QUEUE;

	arch_sched_process_init(proc, entry);

	proc_table[pid] = proc;

	sched_atomic_leave();

	return proc;
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
