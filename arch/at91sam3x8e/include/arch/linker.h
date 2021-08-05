/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <stdint.h>

/**
 * @file linker.h
 * @brief External symbols defined by the linker script (see flash.ld).
 * The declarations in this file don't carry an actual value and cannot be
 * accessed directly; they are merely there to get the *addresses* of.
 */

/** @brief Start of fixed area */
extern uint32_t _sfixed;
/** @brief End of fixed area */
extern uint32_t _efixed;

/** @brief End of program code (.text) */
extern uint32_t _etext;

/** @brief Start of relocation section (for global and static variables) */
extern uint32_t _srelocate;
/** @brief End of relocation section */
extern uint32_t _erelocate;

/** @brief Start of zero area (.bss) */
extern uint32_t _szero;
/** @brief End of zero area (.bss) */
extern uint32_t _ezero;

/**
 * @brief Lowest address of the main (kernel) stack.
 * This is technically the end of the stack, because it grows
 * from the highest address downwards.
 */
extern uint32_t _sstack;
/** @brief Highest address if the main (kernel) stack. */
extern uint32_t _estack;

/** @brief Start of heap area */
extern uint32_t _sheap;
/** @brief End of heap area */
extern uint32_t _eheap;

/**
 * @brief Start of preinit array.
 * The preinit array is an array of function pointers that are invoked before
 * the ones in the init array and `main()` are called.  Only the most critical
 * setup routines, like initializing the memory allocator, should go here.
 */
extern uintptr_t __preinit_array_start;
/** @brief End of preinit array. */
extern uintptr_t __preinit_array_end;

/**
 * @brief Start of init array.
 * The init array is an array of function pointers that are invoked before
 * calling `main()` and are there to initialize the various components of the
 * standard C library.  They are executed after the preinit array, so memory
 * allocation is already available.
 */
extern uint32_t __init_array_start;
/** @brief End of init array. */
extern uint32_t __init_array_end;

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
