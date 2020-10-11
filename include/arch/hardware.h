/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

/**
 * Block the CPU by continuously checking the same expression in an
 * infinite loop, until the condition is true.  Useful for polling.
 *
 * @param expr The expression.
 */
#define mom_are_we_there_yet(expr) ({ while (!(expr)); })

/**
 * Initialize the system hardware.
 * This function is responsible for putting the entire system to a state that
 * allows the Kernel to perform its bootstrap procedure and is therefore the
 * first thing to be called by `do_bootstrap`.  Possible tasks to be dealt with
 * here include:
 *
 * - Performing sanity checks to see if there are any major hardware faults
 * - Setting up the CPU frequency and other oscillators
 * - Communicating that frequency change to any hardware component that needs
 *   to know about it (especially the flash controller)
 * - Enabling interrupts that are vital for stable operation
 *
 * If any of this fails, a nonzero value must be returned to indicate the error
 * condition.  An on-chip LED should be used to "morse" some kind of diagnostic
 * message, if the system has one (kind of like BIOS beep codes).
 *
 * @returns `0` on success, a negative POSIX error code if applicable, or a
 *	positive (platform-dependant) number on hardware fault.
 */
int sys_init(void);

#if defined(ARCH_ATMEGA328P)
#error "ATmega328p is not implemented (yet?)"
#elif defined(ARCH_AT91SAM3X8E)
#include <arch/at91sam3x8e/hardware.h>
#else
#error "Unsupported architecture"
#endif

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
