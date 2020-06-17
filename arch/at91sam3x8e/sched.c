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
#include <ardix/string.h>
#include <ardix/sched.h>
#include <stdbool.h>
#include <toolchain.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Set the PENDSV bit in the system control block.
 */
static __always_inline void sched_pendsv_req(void)
{
	REG_SCB_ICSR |= REG_SCB_ICSR_PENDSVSET_BIT;
}

void isr_sys_tick(void)
{
	/*
	 * fire a PendSV interrupt and do the actual context switching there
	 * because it is faster that way (according to the docs, at least)
	 */
	sched_pendsv_req();
}

void sched_init_process_regs(struct reg_snapshot *reg_snap, void (*entry)(void))
{
	memset(reg_snap, 0, sizeof(*reg_snap));

	reg_snap->hw.lr = (void *)0xFFFFFFF9U;
	reg_snap->hw.pc = entry;
	reg_snap->hw.psr = 0x01000000U;
}

/**
 * Set the NVIC priority grouping field in `AIRCR`.
 * Only values from 0..7 are allowed, see the SAM3X documentation.
 *
 * @param prio_group: The new priority grouping value.
 */
static inline void sched_nvic_set_prio_group(uint32_t prio_group)
{
	uint32_t reg_val = REG_SCB_AIRCR;

	reg_val &= ~(REG_SCB_AIRCR_VECTKEY_MASK | REG_SCB_AIRCR_PRIGROUP_MASK);
	reg_val = reg_val
		| REG_SCB_AIRCR_VECTKEY_VAL(REG_SCB_AIRCR_VECTKEY_MAGIC)
		| REG_SCB_AIRCR_PRIGROUP_VAL(prio_group);

	REG_SCB_AIRCR = reg_val;
}

int sched_hwtimer_init(unsigned int freq)
{
	uint32_t ticks = sys_core_clock / freq;
	if (ticks > REG_SYSTICK_LOAD_RELOAD_MASK)
		return 1;

	/* Ensure SysTick and PendSV are preemptive */
	sched_nvic_set_prio_group(0b011);

	REG_SYSTICK_LOAD = (ticks & REG_SYSTICK_LOAD_RELOAD_MASK) - 1;
	REG_SYSTICK_VAL = 0U;
	REG_SYSTICK_CTRL = REG_SYSTICK_CTRL_CLKSOURCE_BIT /* MCK */
			 | REG_SYSTICK_CTRL_TICKINT_BIT /* trigger exception */
			 | REG_SYSTICK_CTRL_ENABLE_BIT; /* enable SysTick */

	return 0;
}

inline void sched_atomic_enter(void)
{
	REG_SYSTICK_CTRL &= ~REG_SYSTICK_CTRL_ENABLE_BIT;
}

inline void sched_atomic_leave(bool resched)
{
	REG_SYSTICK_CTRL |= REG_SYSTICK_CTRL_ENABLE_BIT;
}

void sched_exec_early(void)
{
	REG_SYSTICK_VAL = 0U; /* Reset timer */
	sched_pendsv_req();
}

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
