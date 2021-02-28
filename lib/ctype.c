/* SPDX-License-Identifier: GPL-3.0-or-later */
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
