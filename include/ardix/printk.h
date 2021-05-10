/* See the end of this file for copyright, license, and warranty information. */

#pragma once

/**
 * Print to the kernel log buffer.
 *
 * @param fmt: A printf-style format string.
 * @return The amount of bytes written, or a negative POSIX number
 */
__attribute__(( format(printf, 1, 2) ))
int printk(const char *fmt, ...);

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
