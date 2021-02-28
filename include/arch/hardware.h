/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/arch_include.h>

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
 * If any of this fails, an on-chip LED should be used to "morse" some kind of
 * diagnostic message, if the system has one (kind of like BIOS beep codes).
 */
void sys_init(void);

#ifndef STACK_SIZE
/** stack size per process in bytes */
#define STACK_SIZE 2048U
#endif

#include ARCH_INCLUDE(hardware.h)

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
