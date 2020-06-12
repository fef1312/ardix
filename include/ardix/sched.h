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

#pragma once

#include <ardix/types.h>
#include <arch/hardware.h>

#ifndef SCHED_MAXPROC
/** The maximum number of processes. */
#define SCHED_MAXPROC 16
#endif /* SCHED_MAXPROC */

#if SCHED_MAXPROC > 64
#warning "SCHED_MAXPROC is > 64, this could have a significant impact on performance"
#endif /* SCHED_MAXPROC > 64 */

enum proc_state {
	/** Process is dead / ddoesn't exist */
	PROC_DEAD,
	/** Process is ready for execution or currently running. */
	PROC_READY,
	/** Process is waiting for its next time share. */
	PROC_QUEUE,
	/** Process is sleeping, `sleep_until` specifies when to wake it up. */
	PROC_SLEEP,
};

/** Stores an entire process image. */
struct process {
	/** Next process in the (circular) list. */
	struct process *next;
	/** Stack pointer. */
	void *sp;
	/** Bottom of the stack (i.e. the highest address). */
	void *stack_bottom;
	/** If `state` is `PROC_SLEEP`, the last execution time. */
	unsigned long int lastexec;
	/** If `state` is `PROC_SLEEP`, the amount of us to sleep in total. */
	unsigned long int sleep_usecs;
	/** Process ID. */
	pid_t pid;
	/** Process state. */
	enum proc_state state;
};

/** The currently executing process. */
extern struct process *_current_process;

/**
 * Initialize the scheduler subsystem.
 * This sets up a hardware interrupt timer (SysTick for Cortex-M3).
 */
int sched_init(void);

/**
 * Switch to the next process (atomic / interrupt handler context only).
 * Must be called directly from within an interrupt routine.
 * Takes care of the following stuff:
 *
 * - Selecting a new process to run, or putting the CPU to sleep
 * - Updating the `_current_process` pointer
 * - Setting the `state` member of both the old and new process to the
 *   appropriate value
 *
 * To avoid the horrors of inline assembly, the stack pointer is modified in
 * actual assembler files rather than here.
 *
 * @param curr_sp: The stack pointer of the current process.
 * @returns The stack pointer of the new process.
 */
void *sched_process_switch(void *curr_sp);

/**
 * Create a new process.
 *
 * @param exec: The process executor.
 * @returns A pointer to the new process, or `NULL` if something went wrong.
 *
 * TODO: make something like errno to tell what *exactly* went wrong
 */
struct process *sched_process_create(void (*exec)(void));

/**
 * Suspend the current process for the specified amount of milliseconds.
 * Note that there are slight deviations from this time interval because of the
 * round-robin scheduling algorithm.
 * If the sleep time is required to be exactly accurate, use `atomic_udelay()`.
 * Note, however, that this will block *all* other processes, even including
 * I/O, for the entire time period.
 *
 * @param msecs: The amount of milliseconds to (approximately) sleep for.
 */
void sleep(unsigned long int msecs);

/**
 * Block the entire CPU from execution for the specified amount of microseconds.
 * Note that this will temporarily disable the scheduler, meaning that *nothing*
 * (not even I/O) will be executed.  The only reason you would ever want to use
 * this is for mission-critical, very short (<= 100 us) periods of time.
 *
 * @param usecs: The amount of microseconds to halt the CPU for.
 */
void atomic_udelay(unsigned long int usecs);
