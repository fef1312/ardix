/* See the end of this file for copyright, license, and warranty information. */

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
 * If any of this fails, an on-chip LED should be used to "morse" some kind of
 * diagnostic message, if the system has one (kind of like BIOS beep codes).
 */
void sys_init(void);

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
