/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <arch/serial.h>

#include <ardix/dma.h>
#include <ardix/printk.h>
#include <ardix/ringbuf.h>
#include <ardix/serial.h>

#include <errno.h>
/* Using GCC's stdarg.h is recommended even with -nodefaultlibs and -fno-builtin */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

#ifndef CONFIG_PRINTK_BUFSZ
#define CONFIG_PRINTK_BUFSZ 64
#endif

/*
 * TODO: THIS CAUSES A STACK BUFFER OVERFLOW ON SYSTEMS WHERE INT IS 64 BITS
 */

/* 10 decimal digits of 4294967295 (2 ** 32 - 1) */
#define PRINTK_UINT_BUFSZ 10

static ssize_t printk_flush(struct ringbuf *buf)
{
	ssize_t ret;
	struct dmabuf *dma = dmabuf_create(&serial_default_device->device, buf->len);
	if (dma == NULL)
		return -ENOMEM;

	ringbuf_read(dma->data, buf, buf->len);
	ret = serial_write_dma(serial_default_device, dma);
	dmabuf_put(dma);
	return ret;
}

__rodata static const char fmt_hex_table[] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f',
};

static int fmt_handle_ptr(struct ringbuf *buf, uintptr_t ptr)
{
	int ret;
	/* 2 chars per byte, plus 2 for the "0x" hex prefix */
	char str[2 * sizeof(uintptr_t) + 2];
	char *pos = &str[2 * sizeof(uintptr_t) + 1];

	str[0] = '0';
	str[1] = 'x';

	do {
		*pos-- = fmt_hex_table[ptr & 0xf];
		ptr >>= 4;
	} while (pos != &str[2]);

	ret = ringbuf_write(buf, &str[0], 2 * sizeof(uintptr_t) + 2);
	return ret;
}

static int fmt_handle_uint(struct ringbuf *buf, unsigned int u)
{
	int ret;
	char str[PRINTK_UINT_BUFSZ];
	char *pos = &str[PRINTK_UINT_BUFSZ - 1];

	do {
		/* stupid big endian humans, forcing us to do the whole thing in reverse */
		*pos-- = (char)(u % 10) + '0'; /* convert to ASCII */
		u /= 10;
	} while (u != 0);
	pos++;

	ret = ringbuf_write(buf, pos, PRINTK_UINT_BUFSZ - (pos - &str[0]));
	return ret;
}

static inline int fmt_handle_int(struct ringbuf *buf, int i)
{
	int ret = 0;
	char minus = '-';

	if (i < 0) {
		ret = ringbuf_write(buf, &minus, sizeof(minus));
		i = -i;
	}

	ret += fmt_handle_uint(buf, i);

	return ret;
}

/**
 * Parse a formatting escape sequence, fetch the parameter from the `va_arg`
 * stack, and print the resulting string to the standard serial console.
 *
 * @param pos: A reference to the pointer to the fmt sequence beginner (`%`).
 *	This is updated to point to the first character *after* the entire
 *	escape sequence.
 * @param args: A pointer to the varargs list.  Will be manipulated.
 * @returns The amount of bytes written, or a negative POSIX error code.
 */
static inline int fmt_handle(struct ringbuf *buf, const char **pos, va_list args)
{
	int ret = 0;
	union {
		int c;
		int d;
		uintptr_t p;
		char *s;
		unsigned int u;
	} val;

	switch (**pos) {
	case '%': /* literal percent sign */
		ret = ringbuf_write(buf, *pos, sizeof(**pos));
		break;

	case 'c': /* char */
		val.c = va_arg(args, typeof(val.c));
		ret = ringbuf_write(buf, &val.c, sizeof(val.c));
		break;

	case 'd': /* int */
		val.d = va_arg(args, typeof(val.d));
		ret = fmt_handle_int(buf, val.d);
		break;

	case 'p': /* ptr */
		val.p = va_arg(args, typeof(val.p));
		ret = fmt_handle_ptr(buf, val.p);
		break;

	case 's': /* string */
		val.s = va_arg(args, typeof(val.s));
		ret = (int)strlen(val.s);
		ret = ringbuf_write(buf, val.s, (size_t)ret);
		break;

	case 'u': /* unsigned int */
		val.u = va_arg(args, typeof(val.u));
		ret = fmt_handle_uint(buf, val.u);
		break;
	}

	(*pos)++;

	return ret;
}

int printk(const char *fmt, ...)
{
	va_list args;
	int ret = 0;
	int tmpret;
	const char *tmp = fmt;
	struct ringbuf *buf = ringbuf_create(CONFIG_PRINTK_BUFSZ);

	if (buf == NULL)
		return -ENOMEM;

	va_start(args, fmt);

	while (*tmp != '\0') {
		if (*tmp++ == '%') {
			/* flush out everything we have so far (minus one char for %) */
			ret += (int)ringbuf_write(buf, fmt, (size_t)tmp - (size_t)fmt - 1);

			tmpret = fmt_handle(buf, &tmp, args);
			/*
			 * act as if the current position were the beginning in
			 * order to make the first step of this if block easier
			 */
			fmt = tmp;

			if (tmpret < 0)
				break;
			ret += tmpret;
		}
	}

	if (tmp != fmt && ret >= 0)
		ret += ringbuf_write(buf, fmt, (size_t)tmp - (size_t)fmt);

	va_end(args);

	printk_flush(buf);
	ringbuf_destroy(buf);

	return ret;
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
