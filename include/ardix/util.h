/* SPDX-License-Identifier: BSD-3-Clause */
/* See the end of this file for copyright, licensing, and warranty information. */

#pragma once

#include <stddef.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

/**
 * Cast a pointer to a member of a struct out to the containing structure.
 *
 * @param ptr A pointer to the nested struct.
 * @param type The type of the containing struct.
 * @param member Name of the member within the containing struct.
 * @return A pointer to the containing struct.
 */
#define container_of(ptr, type, member) \
	( (type *)((void *)(ptr) - offsetof(type, member)) )

/**
 * Get the size of a struct member.
 *
 * @param type The type of the containing struct.
 * @param member The name of the member within the struct.
 * @return The size of the member in bytes.
 */
#define SIZEOF_MEMBER(type, member) \
	(sizeof( ((type *)0)->member ))

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
