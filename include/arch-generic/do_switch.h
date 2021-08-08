/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <arch/hardware.h>
#include <ardix/sched.h>

#include <toolchain.h>

/**
 * @brief New tasks point their initial program counter here.
 */
extern void _leave(void);

extern void _do_switch(struct context *old, struct context *new);

/**
 * @brief Perform an in-kernel context switch.
 * This is called from `schedule()`, and will suspend the current task.
 * The call returns when the current task is woken back up.
 *
 * @param old Currently running task in whose TCB the context will be stored
 * @param new Task to switch to, must not be the same as `old`
 */
__always_inline void do_switch(struct task *old, struct task *new)
{
	_do_switch(&old->tcb.context, &new->tcb.context);
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
 */
