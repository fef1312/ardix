/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#include <ardix/string.h>
#include <ardix/types.h>

#include <toolchain.h>
#include <stdbool.h>

#ifndef __HAVE_ASM_MEMCMP
__shared int memcmp(const void *s1, const void *s2, size_t n)
{
	int delta = 0;

	while (n-- > 0) {
		delta = *(const unsigned char *)s1++ - *(const unsigned char *)s2++;
		if (delta != 0)
			break;
	}

	return delta;
}
#endif /* __HAVE_ASM_MEMCMP */

#ifndef __HAVE_ASM_MEMCPY
__shared void *memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *tmp = (uint8_t *)dest;

	while (n-- > 0)
		*tmp++ = *(const uint8_t *)src++;

	return dest;
}
#endif /* __HAVE_ASM_MEMCPY */

#ifndef __HAVE_ASM_MEMSET
__shared void *memset(void *ptr, int c, size_t n)
{
	char *tmp = (char *)ptr;

	while (n-- > 0)
		*tmp++ = (char)c;

	return ptr;
}
#endif /* __HAVE_ASM_MEMSET */

#ifndef __HAVE_ASM_MEMMOVE
__shared void *memmove(void *dest, const void *src, size_t n)
{
	char *tmp = (char *)dest;
	const char *s = (const char *)src;

	if (dest == src)
		return dest;

	if (dest < src) {
		while (n-- != 0)
			*tmp++ = *s++;
	} else {
		tmp += n;
		s += n;
		while (n-- != 0)
			*--tmp = *--s;
	}

	return dest;
}
#endif /* __HAVE_ASM_MEMMOVE */

#ifndef __HAVE_ASM_STRCMP
__shared int strcmp(const char *s1, const char *s2)
{
	while (*s1++ == *s2++) {
		if (*s1 == '\0' || *s2 == '\0')
			break;
	}

	return *((const unsigned char *)s1) - *((const unsigned char *)s2);
}
#endif /* __HAVE_ASM_STRCMP */

#ifndef __HAVE_ASM_STRCPY
__shared char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*tmp++ = *src++) != '\0');
		/* nothing */

	return dest;
}
#endif /* __HAVE_ASM_STRCPY */

#ifndef __HAVE_ASM_STRNCPY
__shared char *strncpy(char *dest, const char *src, size_t n)
{
	char *tmp = dest;

	while (n-- != 0) {
		if ((*tmp++ = *src++) == '\0')
			break;
	}

	return dest;
}
#endif /* __HAVE_ASM_STRNCPY */

#ifndef __HAVE_ASM_STRLEN
__shared size_t strlen(const char *s)
{
	const char *tmp = s;

	while (*tmp++ != '\0');
		/* nothing */

	return (size_t)tmp - (size_t)s - (size_t)1;
}
#endif /* __HAVE_ASM_STRLEN */

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
