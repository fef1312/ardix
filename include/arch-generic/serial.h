/* See the end of this file for copyright, license, and warranty information. */

#pragma once

#include <ardix/dma.h>
#include <ardix/serial.h>

int arch_serial_init(struct serial_device *dev);
void arch_serial_exit(struct serial_device *dev);

/**
 * Copy `buf` to a hardware buffer in the TX queue.
 * The transmission is performed asynchronously.
 *
 * @param dev: serial device to enqueue the buffer for
 * @param buf: raw buffer data
 * @param len: length of `buf`
 * @returns actual amount of bytes enqueued, or a negative error code on failure
 */
ssize_t arch_serial_write(struct serial_device *dev, const void *buf, size_t len);

/**
 * Directly enqueue a DMA buffer to a serial device, resulting in a zero-copy
 * write.  This will increment the buffer's refcount and decrement it again when
 * it has been written out completely, so the caller is responsible for calling
 * `dmabuf_put` as well in order to prevent a memory leak.
 *
 * @param dev: serial device to write to
 * @param buf: raw DMA buffer to append
 * @returns actual bytes that will be written, or a negative error code
 */
ssize_t serial_write_dma(struct serial_device *dev, struct dmabuf *buf);

#include <arch/serial.h>

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
