/* See the end of this file for copyright, license, and warranty information. */

.syntax unified
.thumb
.cpu cortex-m3

.macro func_begin name
	.global \name
	.type \name, "function"
\name :
.endm

.macro func_end name
	.size \name, .-\name
.endm

/*
 * This file is part of Ardix.
 * Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
 *
 * Ardix is non-violent software: you may only use, redistribute,
 * and/or modify it under the terms of the CNPLv6+ as found in
 * the LICENSE file in the source code root directory or at
 * <https://git.pixie.town/thufie/CNPL>.
 *
 * Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
 * permitted by applicable law.  See the CNPLv6+ for details.
 */
