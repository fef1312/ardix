/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>

typedef uint32_t word_t;

/** Current system frequency in Hertz. */
extern volatile uint32_t SystemCoreClock;

/**
 * All registers that are automatically saved by hardware routines when entering
 * an IRQ, in the correct order.
 */
struct reg_hw_snapshot {
	word_t r0;
	word_t r1;
	word_t r2;
	word_t r3;
	word_t r12;
	void *lr;	/* alias r14 */
	void *pc;	/* alias r15 */
	word_t psr;
};

struct reg_snapshot {
	word_t r0;
	word_t r1;
	word_t r2;
	word_t r3;
	word_t r4;
	word_t r5;
	word_t r6;
	word_t r7;
	word_t r8;
	word_t r9;
	word_t r10;
	word_t r11;

	/*
	 * ATTENTION: the following registers might actually be stored on the
	 *            other stack; don't access them unless you know exactly
	 *            what you're doing
	 */

	word_t _r12;
	void *_lr;	/* alias r14 */
	void *_pc;	/* alias r15 */
	word_t _psr;
};

#define arch_syscall_num(reg_snap) ((reg_snap)->r7)
#define arch_syscall_arg1(reg_snap) ((reg_snap)->r0)
#define arch_syscall_arg2(reg_snap) ((reg_snap)->r1)
#define arch_syscall_arg3(reg_snap) ((reg_snap)->r2)
#define arch_syscall_arg4(reg_snap) ((reg_snap)->r3)
#define arch_syscall_arg5(reg_snap) ((reg_snap)->r4)
#define arch_syscall_arg6(reg_snap) ((reg_snap)->r5)

#define arch_syscall_set_rval(reg_snap, val) ((reg_snap)->r0 = (word_t)(val));

#ifdef ARDIX_ARCH
#	define __SAM3X8E__
#	define DONT_USE_CMSIS_INIT
#	define __PROGRAM_START
#	include <sam3x8e.h>
#endif /* ARDIX_ARCH */

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
