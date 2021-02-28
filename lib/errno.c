/* SPDX-License-Identifier: GPL-3.0-or-later */
/* See the end of this file for copyright, license, and warranty information. */

#include <ardix/util.h>

#include <errno.h>
#include <toolchain.h>

__rodata static const char *error_messages[] = {
	[EPERM]			"Operation not permitted",
	[ENOENT]		"No such file or directory",
	[ESRCH]			"No such process",
	[EINTR]			"Interrupted system call",
	[EIO]			"Input/output error",
	[ENXIO]			"No such device or address",
	[E2BIG]			"Argument list too long",
	[ENOEXEC]		"Exec format error",
	[EBADF]			"Bad file descriptor",
	[ECHILD]		"No child process",
	[EAGAIN]		"Resource temporarily unavailable",
	[ENOMEM]		"Cannot allocate memory",
	[EACCES]		"Permission denied",
	[EFAULT]		"Bad address",
	[ENOTBLK]		"Block device required",
	[EBUSY]			"Device or resource busy",
	[EEXIST]		"File exists",
	[EXDEV]			"Invalid cross-device link",
	[ENODEV]		"No such device",
	[ENOTDIR]		"Not a directory",
	[EISDIR]		"Is a directory",
	[EINVAL]		"Invalid argument",
	[ENFILE]		"Too many open files in system",
	[EMFILE]		"Too many open files",
	[ENOTTY]		"Inappropriate ioctl for device",
	[ETXTBSY]		"Text file busy",
	[EFBIG]			"File too large",
	[ENOSPC]		"No space left on device",
	[ESPIPE]		"Illegal seek",
	[EROFS]			"Read-only file system",
	[EMLINK]		"Too many links",
	[EPIPE]			"Broken pipe",
	[EDOM]			"Numerical argument out of domain",
	[ERANGE]		"Numerical result out of range",
	[EDEADLK]		"Resource deadlock avoided",
	[ENAMETOOLONG]		"File name too long",
	[ENOLCK]		"No locks available",
	[ENOSYS]		"Function not implemented",
	[ENOTEMPTY]		"Directory not empty",
	[ELOOP]			"Too many levels of symbolic links",
	[EWOULDBLOCK]		"Resource temporarily unavailable",
	[ENOMSG]		"No message of desired type",
	[EIDRM]			"Identifier removed",
	[ECHRNG]		"Channel number out of range",
	[EL2NSYNC]		"Level 2 not synchronized",
	[EL3HLT]		"Level 3 halted",
	[EL3RST]		"Level 3 reset",
	[ELNRNG]		"Link number out of range",
	[EUNATCH]		"Protocol driver not attached",
	[ENOCSI]		"No CSI structure available",
	[EL2HLT]		"Level 2 halted",
	[EBADE]			"Invalid exchange",
	[EBADR]			"Invalid request descriptor",
	[EXFULL]		"Exchange full",
	[ENOANO]		"No anode",
	[EBADRQC]		"Invalid request code",
	[EBADSLT]		"Invalid slot",
	[EDEADLOCK]		"Resource deadlock avoided",
	[EBFONT]		"Bad font file format",
	[ENOSTR]		"Device not a stream",
	[ENODATA]		"No data available",
	[ETIME]			"Timer expired",
	[ENOSR]			"Out of streams resources",
	[ENONET]		"Machine is not on the network",
	[ENOPKG]		"Package not installed",
	[EREMOTE]		"Object is remote",
	[ENOLINK]		"Link has been severed",
	[EADV]			"Advertise error",
	[ESRMNT]		"Srmount error",
	[ECOMM]			"Communication error on send",
	[EPROTO]		"Protocol error",
	[EMULTIHOP]		"Multihop attempted",
	[EDOTDOT]		"RFS specific error",
	[EBADMSG]		"Bad message",
	[EOVERFLOW]		"Value too large for defined data type",
	[ENOTUNIQ]		"Name not unique on network",
	[EBADFD]		"File descriptor in bad state",
	[EREMCHG]		"Remote address changed",
	[ELIBACC]		"Can not access a needed shared library",
	[ELIBBAD]		"Accessing a corrupted shared library",
	[ELIBSCN]		".lib section in a.out corrupted",
	[ELIBMAX]		"Attempting to link in too many shared libraries",
	[ELIBEXEC]		"Cannot exec a shared library directly",
	[EILSEQ]		"Invalid or incomplete multibyte or wide character",
	[ERESTART]		"Interrupted system call should be restarted",
	[ESTRPIPE]		"Streams pipe error",
	[EUSERS]		"Too many users",
	[ENOTSOCK]		"Socket operation on non-socket",
	[EDESTADDRREQ]		"Destination address required",
	[EMSGSIZE]		"Message too long",
	[EPROTOTYPE]		"Protocol wrong type for socket",
	[ENOPROTOOPT]		"Protocol not available",
	[EPROTONOSUPPORT]	"Protocol not supported",
	[ESOCKTNOSUPPORT]	"Socket type not supported",
};

char *strerror(int errnum)
{
	if (errnum < 0)
		errnum = -errnum;

	if ((unsigned int)errnum >= ARRAY_SIZE(error_messages))
		return NULL;

	/*
	 * POSIX prohibits modifying the string returned by strerror(), so we
	 * can safely cast here.  If userland is stupid enough to write to it
	 * anyways, they will get a segfault because the messages are stored
	 * in ROM rather than RAM.
	 */
	return (char *)error_messages[errnum];
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
