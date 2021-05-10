/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/types.h>

#include <stdbool.h>
#include <string.h>
#include <toolchain.h>

#ifndef __HAVE_ASM_MEMCMP
int memcmp(const void *s1, const void *s2, size_t n)
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
void *memcpy(void *dest, const void *src, size_t n)
{
	uint8_t *tmp = (uint8_t *)dest;

	while (n-- > 0)
		*tmp++ = *(const uint8_t *)src++;

	return dest;
}
#endif /* __HAVE_ASM_MEMCPY */

#ifndef __HAVE_ASM_MEMSET
void *memset(void *ptr, int c, size_t n)
{
	char *tmp = (char *)ptr;

	while (n-- > 0)
		*tmp++ = (char)c;

	return ptr;
}
#endif /* __HAVE_ASM_MEMSET */

#ifndef __HAVE_ASM_MEMMOVE
void *memmove(void *dest, const void *src, size_t n)
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
int strcmp(const char *s1, const char *s2)
{
	while (*s1++ == *s2++) {
		if (*s1 == '\0' || *s2 == '\0')
			break;
	}

	return *((const unsigned char *)s1) - *((const unsigned char *)s2);
}
#endif /* __HAVE_ASM_STRCMP */

#ifndef __HAVE_ASM_STRCPY
char *strcpy(char *dest, const char *src)
{
	char *tmp = dest;

	while ((*tmp++ = *src++) != '\0');
		/* nothing */

	return dest;
}
#endif /* __HAVE_ASM_STRCPY */

#ifndef __HAVE_ASM_STRNCPY
char *strncpy(char *dest, const char *src, size_t n)
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
size_t strlen(const char *s)
{
	const char *tmp = s;

	while (*tmp++ != '\0');
		/* nothing */

	return (size_t)tmp - (size_t)s - (size_t)1;
}
#endif /* __HAVE_ASM_STRLEN */

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
