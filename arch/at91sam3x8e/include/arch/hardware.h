/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>
#include <toolchain.h>

typedef uint32_t word_t;
typedef uint32_t sysarg_t;

/** Current system frequency in Hertz. */
extern volatile uint32_t SystemCoreClock;

/**
 * @brief Hardware context save upon entering kernel space.
 * This is stored on the stack of the thread that ran before exception entry,
 * i.e. PSP if user space and MSP if kernel space.
 */
struct hw_context {
	word_t r0;
	word_t r1;
	word_t r2;
	word_t r3;
	word_t r12;
	void *lr;	/* alias r14 */
	void *pc;	/* alias r15 */
	word_t psr;
};

/**
 * @brief Software context save from an exception handler upon entering kernel space.
 * This is always stored on the main stack.
 * The `prepare_entry` macro in `arch/include/asm.S` creates this snapshot.
 */
struct exc_context {
	word_t r4;
	word_t r5;
	word_t r6;
	word_t r7;
	word_t r8;
	word_t r9;
	word_t r10;
	word_t r11;
	/*
	 * Old stack pointer used before exception entry.
	 * Bit 2 in lr defines which stack was used.
	 */
	struct hw_context *sp;
	void *lr;
};

/**
 * @brief Used for in-kernel context switching.
 * This is where `do_switch()` stores the register values.
 */
struct context {
	word_t r4;
	word_t r5;
	word_t r6;
	word_t r7;
	word_t r8;
	word_t r9;
	word_t r10;
	word_t r11;
	void *sp;
	void *pc;
};

/**
 * @brief Task Control Block.
 * This is a low level structure used by `do_switch()` to do the actual context
 * switching, and embedded into `struct task`.  We do this nesting because it
 * makes it easier to access the TCB's fields from assembly, and it also makes
 * us less dependent on a specific architecture.
 */
struct tcb {
	struct context context;
	/*
	 * Needed for exec() because the child task leaves kernel space over a
	 * different route than the parent one.
	 */
	struct exc_context *exc_context;
};

__always_inline sysarg_t sc_num(const struct exc_context *ctx)
{
	return ctx->r7;
}

__always_inline sysarg_t sc_arg1(const struct exc_context *ctx)
{
	return ctx->sp->r0;
}

__always_inline sysarg_t sc_arg2(const struct exc_context *ctx)
{
	return ctx->sp->r1;
}

__always_inline sysarg_t sc_arg3(const struct exc_context *ctx)
{
	return ctx->sp->r2;
}

__always_inline sysarg_t sc_arg4(const struct exc_context *ctx)
{
	return ctx->sp->r3;
}

__always_inline sysarg_t sc_arg5(const struct exc_context *ctx)
{
	return ctx->r4;
}

__always_inline sysarg_t sc_arg6(const struct exc_context *ctx)
{
	return ctx->r5;
}

__always_inline void sc_set_rval(struct exc_context *ctx, long rval)
{
	/* raw cast */
	*(long *)&ctx->sp->r0 = rval;
}

#ifdef ARDIX_ARCH
#	define __SAM3X8E__
#	define DONT_USE_CMSIS_INIT
#	define __PROGRAM_START

#	include <sam3x8e.h>

#	undef __PROGRAM_START
#	undef DONT_USE_CMSIS_INIT
#	undef __SAM3X8E__
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
