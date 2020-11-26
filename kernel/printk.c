/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

/* Using GCC's stdarg.h is recommended even with -nodefaultlibs and -fno-builtin */
#include <stdarg.h>

#include <ardix/printk.h>
#include <ardix/serial.h>
#include <ardix/string.h>

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
static inline int handle_fmt(const char **pos, va_list *args)
{
	int ret = 0;
	int c;
	char *tmp;

	switch (**pos) {
	case '%': /* literal percent sign */
		serial_write(serial_default_interface, *pos, sizeof(**pos));
		ret = sizeof(**pos);
		break;

	case 'c': /* single char */
		c = va_arg(*args, int);
		serial_write(serial_default_interface, &c, sizeof(char));
		break;

	case 's': /* string */
		tmp = va_arg(*args, char *);
		ret = (int)strlen(tmp);
		serial_write(serial_default_interface, tmp, (size_t)ret);
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
		ret++;

		if (*tmp++ == '%') {
			/* flush out everything we have so far (minus one char for %) */
			serial_write(serial_default_interface, fmt, (size_t)tmp - (size_t)fmt - 1);

			tmpret = handle_fmt(&tmp, &args);
			if (tmpret < 0) {
				fmt = tmp;
				break;
			}

			/*
			 * act as if the current position were the beginning in
			 * order to make the first step of this if block easier
			 */
			ret += tmpret;
			fmt = tmp;
		}
	}

	if (tmp != fmt && ret > 0)
		ret += serial_write(serial_default_interface, fmt, (size_t)tmp - (size_t)fmt);

	va_end(args);
	return ret;
}

/*
 * Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 *    conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
