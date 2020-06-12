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

#include <ardix/clock.h>
#include <ardix/sched.h>
#include <ardix/types.h>
#include <stddef.h>

extern uint32_t _sstack;
extern uint32_t _estack;

struct process *_current_process;

static struct process procs[SCHED_MAXPROC + 1];

int sched_init(void)
{
	int i;

	_current_process = &procs[0];
	_current_process->next = _current_process;
	_current_process->sp = &_sstack;
	_current_process->stack_bottom = &_estack;
	_current_process->pid = 0;
	_current_process->state = PROC_READY;

	for (i = 1; i < SCHED_MAXPROC + 1; i++) {
		procs[i].state = PROC_DEAD;
		procs[i].pid = -1;
	}

	/* TODO: initialize SysTick */

	return 0;
}

void *sched_process_switch(void *curr_sp)
{
	struct process *nextproc = _current_process;
	_current_process->sp = curr_sp;

	while (true) {
		nextproc = nextproc->next;
		if (nextproc->state == PROC_QUEUE) {
			_current_process->state = PROC_QUEUE;
			nextproc->state = PROC_READY;
			_current_process = nextproc;
			break;
		}
	}

	return _current_process->sp;
}
