/* See the end of this file for copyright, license, and warranty information. */

#include <arch/interrupt.h>
#include <arch/hardware.h>
#include <arch-generic/hardware.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

#define SYS_BOARD_OSCOUNT	(CKGR_MOR_MOSCXTST(0x8))
#define SYS_BOARD_MCKR		(PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLLA_CLK)

#define SYS_BOARD_PLLAR (			\
	CKGR_PLLAR_ONE				\
	| CKGR_PLLAR_MULA(0xdUL)		\
	| CKGR_PLLAR_PLLACOUNT(0x3fUL)		\
	| CKGR_PLLAR_DIVA(0x1UL)		\
)

/*
 * The initial sys_core_clock on system boot would actually be 4000000 because
 * the 4 MHz RC oscillator is used by default.  However, for performance
 * reasons, the system core clock is configured to use the 84 MHz PLLA clock
 * even before the memory area for global variables (like this one) is copied
 * from flash to RAM, which in turn would overwrite any updates to the clock.
 * So, since we trust the clock to be configured correctly before this global
 * variable is accessed anywhere, we initialize it to the 84 MHz clock.
 */
volatile uint32_t SystemCoreClock = 84000000UL;

void sys_init(void)
{
	/*
	 * This method is basically an implementation of chapter 28.12 in the
	 * Atmel SAM3X8E Datasheet, combined with the startup code from libsam.
	 */

	/* # of wait states as per hardware spec (stolen from SAM SysInit) */
	EFC0->EEFC_FMR = EEFC_FMR_FWS(4);
	EFC1->EEFC_FMR = EEFC_FMR_FWS(4);

	/*
	 * 1. Enabling the Main Oscillator
	 */

	/* initialize main osc */
	if (!(PMC->CKGR_MOR & CKGR_MOR_MOSCSEL)) {
		PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD
			      | SYS_BOARD_OSCOUNT
			      | CKGR_MOR_MOSCRCEN
			      | CKGR_MOR_MOSCXTEN;
		mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_MOSCXTS);
	}

	/* Switch to 3-20MHz Xtal oscillator */
	PMC->CKGR_MOR = CKGR_MOR_KEY_PASSWD
		      | SYS_BOARD_OSCOUNT
		      | CKGR_MOR_MOSCRCEN
		      | CKGR_MOR_MOSCXTEN
		      | CKGR_MOR_MOSCSEL;
	mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_MOSCSELS);

	/*
	 * 2. Checking the Main Oscillator Frequency (Optional)
	 */

	/* I repeat: **(Optional)** */

	/*
	 * 3. Setting PLL and Divider
	 */

	PMC->PMC_MCKR = (PMC->PMC_MCKR & ~(uint32_t)PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_MCKRDY);

	/*
	 * 4. Selection of Master Clock and Processor Clock
	 */

	/* Initialize PLLA */
	PMC->CKGR_PLLAR = SYS_BOARD_PLLAR;
	mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_LOCKA);

	/* Switch to MCK */
	PMC->PMC_MCKR = (SYS_BOARD_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
	mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_MCKRDY);

	/* Switch to PLLA */
	PMC->PMC_MCKR = SYS_BOARD_MCKR;
	mom_are_we_there_yet(PMC->PMC_SR & PMC_SR_MCKRDY);
}

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
 *
 * ===========================================================================
 *
 * Parts of this file are derived from the Atmel SAM Software Package.
 * Copyright (c) 2015, Atmel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following condition is met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER:  THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
