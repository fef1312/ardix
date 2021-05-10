/* See the end of this file for copyright, license, and warranty information. */

#include <ctype.h>

int isalpha(int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isblank(int c)
{
	return c == ' ' || c == '\t';
}

int iscntrl(int c)
{
	return c == 0x7F || (c >= 0 && c <= 0x1F);
}

int isdigit(int c)
{
	return c >= '0' && c <= '9';
}

int isgraph(int c)
{
	/* space *not* included */
	return c > 0x20 && c <= 0x7E;
}

int islower(int c)
{
	return c >= 'a' && c <= 'z';
}

int isprint(int c)
{
	/* space *is* included */
	return c >= 0x20 && c <= 0x7E;
}

int ispunct(int c)
{
	return isprint(c) && !isalnum(c);
}

int isspace(int c)
{
	return c == ' ' || (c >= '\n' && c <= '\r');
}

int isupper(int c)
{
	return c >= 'A' && c <= 'Z';
}

int isxdigit(int c)
{
	return (c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'F')
		|| (c >= 'a' && c <= 'f');
}

int isascii(int c)
{
	return c & 0x7F;
}

int isalnum(int c)
{
	return isalpha(c) || isdigit(c);
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
