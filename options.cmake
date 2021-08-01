# See the end of this file for copyright and license terms.

option(DEBUG "Enable debug features" ON)

set(ARCH "at91sam3x8e" CACHE STRING "Target architecture")
set_property(CACHE ARCH PROPERTY STRINGS
	"at91sam3x8e"
)
string(TOUPPER ${ARCH} ARCH_UPPERCASE)

set(CONFIG_NFILE 16 CACHE STRING "Maximum number of open files")

set(CONFIG_STACK_SIZE 2048 CACHE STRING "Stack size in bytes")

set(CONFIG_SCHED_MAXTASK 8 CACHE STRING "Maximum number of running tasks")

set(CONFIG_SCHED_FREQ 1000 CACHE STRING "Task switch frequency in Hz")

set(CONFIG_SERIAL_BAUD 115200 CACHE STRING "Default serial baud rate")
set_property(CACHE CONFIG_SERIAL_BAUD PROPERTY STRINGS
	200 2400 4800 9600 19200 38400 57600 115200
)

set(CONFIG_SERIAL_BUFSZ 256 CACHE STRING "Default serial buffer size in bytes")

set(CONFIG_PRINTF_BUFSZ 64 CACHE STRING "Default buffer size for printf() and friends")

option(CONFIG_CHECK_SYSCALL_SOURCE "Prohibit inline syscalls" OFF)

# This file is part of Ardix.
# Copyright (c) 2021 Felix Kopp <owo@fef.moe>.
#
# Ardix is non-violent software: you may only use, redistribute,
# and/or modify it under the terms of the CNPLv6+ as found in
# the LICENSE file in the source code root directory or at
# <https://git.pixie.town/thufie/CNPL>.
#
# Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
# permitted by applicable law.  See the CNPLv6+ for details.
