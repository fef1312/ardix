/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

#include <arch/hardware.h>
#include <arch/sched.h>
#include <ardix/sched.h>
#include <ardix/string.h>
#include <ardix/types.h>
#include <stddef.h>

extern uint32_t _sstack;
extern uint32_t _estack;

struct process *_current_process;

/**
 * An array of all processes.
 * The `pid` not only identifies each process, it is also the index of the
 * struct in this array.  Unused slots have a `pid` of `-1`, however.
 */
static struct process procs[CONFIG_SCHED_MAXPROC + 1];

/**
 * Find an unused process slot in the `procs` array, insert that process into
 * the scheduler's ring queue and return it.  Must run in atomic context.
 *
 * @returns A pointer to the new process slot, or `NULL` if none are available.
 */
static struct process *sched_find_free_slot_and_link(void)
{
	pid_t i;
	struct process *newproc = NULL;

	for (i = 1; i < CONFIG_SCHED_MAXPROC + 1; i++) {
		if (procs[i].pid == -1 && procs[i].state == PROC_DEAD) {
			newproc = &procs[i];
			newproc->next = procs[i - 1].next;
			procs[i - 1].next = newproc;
			newproc->pid = i;
			break;
		}
	}

	return newproc;
}

int sched_init(void)
{
	int i;

	_current_process = &procs[0];
	_current_process->next = _current_process;
	_current_process->sp = &_sstack;
	_current_process->stack_bottom = &_estack;
	_current_process->pid = 0;
	_current_process->state = PROC_READY;

	for (i = 1; i < CONFIG_SCHED_MAXPROC + 1; i++) {
		procs[i].next = NULL;
		procs[i].sp = NULL;
		procs[i].stack_bottom = NULL;
		procs[i].pid = -1;
		procs[i].state = PROC_DEAD;
	}

	i = sched_hwtimer_init(CONFIG_SCHED_INTR_FREQ);

	return i;
}

/**
 * Determine whether the specified process should be executed next.
 *
 * @param proc: The process.
 * @returns Whether `proc` should be next in line for the scheduler.
 */
static inline bool sched_proc_should_run(const struct process *proc)
{
	unsigned long int sleep_usecs;
	unsigned long int lastexec;
	enum proc_state state = proc->state;

	if (state == PROC_QUEUE || state == PROC_READY)
		return true;

	return false;
}

void *sched_process_switch(void *curr_sp)
{
	struct process *nextproc = _current_process;
	_current_process->sp = curr_sp;

	if (_current_process->state != PROC_SLEEP)
		_current_process->state = PROC_QUEUE;

	while (true) {
		nextproc = nextproc->next;
		if (sched_proc_should_run(nextproc)) {
			nextproc->state = PROC_READY;
			_current_process = nextproc;
			break;
		}

		/* TODO: Let the CPU sleep if there is nothing to do */
	}

	return _current_process->sp;
}
