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

#include <arch/sched.h>
#include <arch/at91sam3x8e/hardware.h>

int sched_hwtimer_init(unsigned int freq)
{
	uint32_t ticks = F_CPU / freq;
	if (ticks > REG_SYSTICK_LOAD_RELOAD_MASK)
		return 1;

	REG_SYSTICK_LOAD = (ticks & REG_SYSTICK_LOAD_RELOAD_MASK) - 1;
	REG_SYSTICK_VAL = 0U;
	REG_SYSTICK_CTRL = REG_SYSTICK_CTRL_CLKSOURCE_MASK
			 | REG_SYSTICK_CTRL_TICKINT_MASK
			 | REG_SYSTICK_CTRL_ENABLE_MASK;

	return 0;
}

void sched_hwtimer_pause(void)
{
	REG_SYSTICK_CTRL &= ~REG_SYSTICK_CTRL_ENABLE_MASK;
}

void sched_hwtimer_resume(void)
{
	REG_SYSTICK_CTRL |= REG_SYSTICK_CTRL_ENABLE_MASK;
}
