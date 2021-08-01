/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/malloc.h>

#include <errno.h>
/* Using GCC's stdarg.h is recommended even with -nodefaultlibs and -fno-builtin */
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <config.h>
#include <toolchain.h>

#if __SIZEOF_INT__ == 2
	/* 5 decimal digits of 65535 (2 ** 16 - 1) */
#	define PRINTF_UINT_BUFSZ 5
#elif __SIZEOF_INT__ == 4
	/* 10 decimal digits of 4294967295 (2 ** 32 - 1) */
#	define PRINTF_UINT_BUFSZ 10
#else
	/* 20 decimal digits of 18446744073709551616 (2 ** 64 - 1) */
#	define PRINTF_UINT_BUFSZ 20
#endif

struct printf_buf {
	size_t len;
	int fd;
	uint8_t data[0];
};

static struct printf_buf *printf_buf_create(int fd)
{
	struct printf_buf *buf = malloc(sizeof(*buf) + CONFIG_PRINTF_BUFSZ);

	if (buf != NULL) {
		buf->len = 0;
		buf->fd = fd;
	}

	return buf;
}

static void printf_buf_destroy(struct printf_buf *buf)
{
	if (buf->len != 0)
		write(buf->fd, &buf->data[0], buf->len);

	free(buf);
}

static ssize_t printf_buf_write(struct printf_buf *buf, const void *data, size_t len)
{
	ssize_t ret = 0;
	const uint8_t *tmp = data;

	while (ret != (ssize_t)len) {
		if (buf->len == CONFIG_PRINTF_BUFSZ) {
			/*
			 * TODO: We don't need to take the syscall detour
			 *       if we are already in kernel context
			 */
			ssize_t write_ret = write(buf->fd, &buf->data[0], buf->len);
			if (write_ret < 0) {
				ret = write_ret;
				break;
			}

			if (write_ret == 0) {
				/*
				 * assume something has failed spectacularly
				 * if write() didn't even write a single byte
				 */
				ret = -EIO;
				break;
			}

			if (write_ret != (ssize_t)buf->len)
				memmove(&buf->data[0], &buf->data[write_ret], buf->len - write_ret);

			buf->len -= write_ret;
		}

		buf->data[buf->len++] = *tmp++;
		ret++;
	}

	return ret;
}

static int fmt_handle_ptr(struct printf_buf *buf, uintptr_t ptr)
{
	static const char fmt_hex_table[] = {
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f',
	};

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

	ret = printf_buf_write(buf, &str[0], 2 * sizeof(uintptr_t) + 2);
	return ret;
}

static int fmt_handle_uint(struct printf_buf *buf, unsigned int u)
{
	int ret;
	char str[PRINTF_UINT_BUFSZ];
	char *pos = &str[PRINTF_UINT_BUFSZ - 1];

	do {
		/* stupid big endian humans, forcing us to do the whole thing in reverse */
		*pos-- = (char)(u % 10) + '0'; /* convert to ASCII */
		u /= 10;
	} while (u != 0);
	pos++;

	ret = printf_buf_write(buf, pos, PRINTF_UINT_BUFSZ - (pos - &str[0]));
	return ret;
}

static inline int fmt_handle_int(struct printf_buf *buf, int i)
{
	int ret = 0;
	char minus = '-';

	if (i < 0) {
		ret = printf_buf_write(buf, &minus, sizeof(minus));
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
static inline int fmt_handle(struct printf_buf *buf, const char **pos, va_list args)
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
		ret = printf_buf_write(buf, *pos, sizeof(**pos));
		break;

	case 'c': /* char */
		val.c = va_arg(args, typeof(val.c));
		ret = printf_buf_write(buf, &val.c, sizeof(val.c));
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
		ret = printf_buf_write(buf, val.s, (size_t)ret);
		break;

	case 'u': /* unsigned int */
		val.u = va_arg(args, typeof(val.u));
		ret = fmt_handle_uint(buf, val.u);
		break;
	}

	(*pos)++;

	return ret;
}

int vfprintf(FILE *f, const char *fmt, va_list args)
{
	ssize_t ret = 0;
	const char *tmp = fmt;
	struct printf_buf *buf = printf_buf_create(_file_to_fd(f));

	if (buf == NULL)
		return -ENOMEM;

	while (*tmp != '\0') {
		if (*tmp++ == '%') {
			/* flush out everything we have so far (minus one char for %) */
			ssize_t tmpret = printf_buf_write(buf, fmt, (size_t)tmp - (size_t)fmt - 1);
			if (tmpret < 0) {
				ret = tmpret;
				break;
			}
			ret += tmpret;

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
		ret += printf_buf_write(buf, fmt, (size_t)tmp - (size_t)fmt);

	printf_buf_destroy(buf);

	return (int)ret;
}

int vprintf(const char *fmt, va_list args)
{
	return vfprintf(stdout, fmt, args);
}

int fprintf(FILE *f, const char *fmt, ...)
{
	int ret;
	va_list args;

	va_start(args, fmt);
	ret = vfprintf(f, fmt, args);
	va_end(args);

	return ret;
}

int printf(const char *fmt, ...)
{
	int ret;
	va_list args;

	va_start(args, fmt);
	ret = vfprintf(stdout, fmt, args);
	va_end(args);

	return ret;
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
