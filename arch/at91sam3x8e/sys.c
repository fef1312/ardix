/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/at91sam3x8e/interrupt.h>
#include <arch/hardware.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

/*
 * The initial sys_core_clock on system boot would actually be 4000000 because
 * the 4 MHz RC oscillator is used by default.  However, for performance
 * reasons, the system core clock is configured to use the 84 MHz PLLA clock
 * even before the memory area for global variables (like this one) is copied
 * from flash to RAM, which in turn would overwrite any updates to the clock.
 * So, since we trust the clock to be configured correctly before this global
 * variable is accessed anywhere, we initialize it to the 84 MHz clock.
 */
uint32_t sys_core_clock = 84000000UL;

void sys_init(void)
{
	/*
	 * This method is basically an implementation of chapter 28.12 in the
	 * Atmel SAM3X8E Datasheet, combined with the startup code from libsam.
	 */

	/* # of wait states as per hardware spec (stolen from SAM SysInit) */
	REG_EEFC0_FMR = REG_EEFC_FWS_VAL(4);
	REG_EEFC1_FMR = REG_EEFC_FWS_VAL(4);

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
	mom_are_we_there_yet(REG_PMC_SR & REG_PMC_SR_MOSCSELS_BIT);

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
}

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
