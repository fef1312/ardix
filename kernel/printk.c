/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/printk.h>
#include <ardix/serial.h>

#include <errno.h>
/* Using GCC's stdarg.h is recommended even with -nodefaultlibs and -fno-builtin */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <toolchain.h>

/*
 * TODO: THIS CAUSES A STACK BUFFER OVERFLOW ON SYSTEMS WHERE INT IS 64 BITS
 */

/* 10 decimal digits of 4294967295 (2 ** 32 - 1) */
#define PRINTK_UINT_BUFSZ 10

__rodata static const char fmt_hex_table[] = {
	'0', '1', '2', '3',
	'4', '5', '6', '7',
	'8', '9', 'a', 'b',
	'c', 'd', 'e', 'f',
};

static int fmt_handle_ptr(uintptr_t ptr)
{
	int ret;
	/* 2 chars per byte, plus 2 for the "0x" hex prefix */
	char buf[2 * sizeof(uintptr_t) + 2];
	char *pos = &buf[2 * sizeof(uintptr_t) + 1];

	buf[0] = '0';
	buf[1] = 'x';

	do {
		*pos-- = fmt_hex_table[ptr & 0xf];
		ptr >>= 4;
	} while (pos != &buf[2]);

	ret = serial_write(serial_default_device, &buf[0], 2 * sizeof(uintptr_t) + 2);
	return ret;
}

static int fmt_handle_uint(unsigned int u)
{
	int ret;
	char buf[PRINTK_UINT_BUFSZ];
	char *pos = &buf[PRINTK_UINT_BUFSZ - 1];

	do {
		/* stupid big endian humans, forcing us to do the whole thing in reverse */
		*pos-- = (char)(u % 10) + '0'; /* convert to ASCII */
		u /= 10;
	} while (u != 0);
	pos++;

	ret = serial_write(serial_default_device, pos,
			   PRINTK_UINT_BUFSZ - ( (size_t)pos - (size_t)&buf[0] ));
	return ret;
}

static inline int fmt_handle_int(int i)
{
	int ret = 0;
	char minus = '-';

	if (i < 0) {
		ret = serial_write(serial_default_device, &minus, sizeof(minus));
		i = -i;
	}

	ret += fmt_handle_uint((unsigned int)i);

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
static inline int fmt_handle(const char **pos, va_list args)
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
		ret = serial_write(serial_default_device, *pos, sizeof(**pos));
		break;

	case 'c': /* char */
		val.c = va_arg(args, typeof(val.c));
		ret = serial_write(serial_default_device, &val.c, sizeof(val.c));
		break;

	case 'd': /* int */
		val.d = va_arg(args, typeof(val.d));
		ret = fmt_handle_int(val.d);
		break;

	case 'p': /* ptr */
		val.p = va_arg(args, typeof(val.p));
		ret = fmt_handle_ptr(val.p);
		break;

	case 's': /* string */
		val.s = va_arg(args, typeof(val.s));
		ret = (int)strlen(val.s);
		ret = serial_write(serial_default_device, val.s, (size_t)ret);
		break;

	case 'u': /* unsigned int */
		val.u = va_arg(args, typeof(val.u));
		ret = fmt_handle_uint(val.u);
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

	va_start(args, fmt);

	while (*tmp != '\0') {
		if (*tmp++ == '%') {
			/* flush out everything we have so far (minus one char for %) */
			ret += (int)serial_write(serial_default_device, fmt,
						 (size_t)tmp - (size_t)fmt - 1);

			tmpret = fmt_handle(&tmp, args);
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
		ret += serial_write(serial_default_device, fmt, (size_t)tmp - (size_t)fmt);

	va_end(args);

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
