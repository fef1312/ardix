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

#include <stddef.h>
#include <stdint.h>
#include <arch/at91sam3x8e/interrupt.h>
#include <arch/hardware.h>
#include <ardix/string.h>
#include <toolchain.h>

uint32_t sys_core_clock = 4000000UL; /* default internal 4MHz RC oscillator */

int sys_init(void)
{
	/*
	 * This method is basically an implementation of chapter 28.12 in the
	 * Atmel SAM3X8E Datasheet, combined with the startup code from libsam.
	 */

	/* # of wait states as per hardware spec (stolen from SAM SysInit) */
	REG_EEFC0_FMR = REG_EEFC_FWS_VAL(4);
	REG_EEFC1_FMR = REG_EEFC_FWS_VAL(4);

	/* disable osc write protection */
	REG_PMC_WPMR = REG_PMC_WPMR_WPKEY_VAL(REG_PMC_WPMR_WPKEY_MAGIC)
		     & ~REG_PMC_WPMR_WPEN_BIT;

	/*
	 * 1. Enabling the Main Oscillator
	 */

	/* initialize main osc */
	if (!(REG_CKGR_MOR & REG_CKGR_MOR_MOSCSEL_BIT)) {
		REG_CKGR_MOR = REG_CKGR_MOR_KEY_VAL(REG_CKGR_MOR_KEY_MAGIC)
			     | REG_CKGR_MOR_MOSCXTST_VAL(8)
			     | REG_CKGR_MOR_MOSCRCEN_BIT
			     | REG_CKGR_MOR_MOSCXTEN_BIT;
		mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MOSCXTS_BIT);
	}

	/* switch to Xtal osc */
	REG_CKGR_MOR = REG_CKGR_MOR_KEY_VAL(REG_CKGR_MOR_KEY_MAGIC)
		     | REG_CKGR_MOR_MOSCXTST_VAL(8)
		     | REG_CKGR_MOR_MOSCRCEN_BIT
		     | REG_CKGR_MOR_MOSCXTEN_BIT
		     | REG_CKGR_MOR_MOSCSEL_BIT;
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MOSCXTS_BIT);

	REG_PMC_MCKR = (REG_PMC_MCKR & ~REG_PMC_MCKR_CSS_MASK)
		     | REG_PMC_MCKR_CSS_VAL(1 /* = main clock */);
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MCKRDY_BIT);

	/*
	 * 2. Checking the Main Oscillator Frequency (Optional)
	 */

	/* I repeat: **(Optional)** */

	/*
	 * 3. Setting PLL and Divider
	 */

	REG_CKGR_PLLAR = REG_CKGR_PLLAR_ONE_BIT
		       | REG_CKGR_PLLAR_MULA_VAL(0xD)
		       | REG_CKGR_PLLAR_PLLACOUNT_VAL(0x3F /* maximum value */)
		       | REG_CKGR_PLLAR_DIVA_VAL(0x1);
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_LOCKA_BIT);

	/*
	 * 4. Selection of Master Clock and Processor Clock
	 */

	REG_PMC_MCKR = REG_PMC_MCKR_PRES_VAL(1 /* = as fast as it gets */)
		     | REG_PMC_MCKR_CSS_VAL(1 /* = main clock */);
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MCKRDY_BIT);

	/* PMC_MCKR must not be configured within one clock cycle */
	REG_PMC_MCKR = REG_PMC_MCKR_PRES_VAL(1 /* = as fast as it gets */)
		     | REG_PMC_MCKR_CSS_VAL(2 /* = PLLA clock */);
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MCKRDY_BIT);

	/* turn osc write protection on again */
	REG_PMC_WPMR |= REG_PMC_WPMR_WPEN_BIT;

	sys_core_clock = 84000000UL;
	return 0;
}
