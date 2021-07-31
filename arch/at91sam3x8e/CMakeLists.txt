# See the end of this file for copyright and license terms.

add_library(ardix_arch STATIC)
target_link_libraries(ardix INTERFACE ardix_arch)

target_compile_options(ardix_arch PRIVATE ${ARDIX_COMPILE_OPTIONS})
target_include_directories(ardix_arch PRIVATE ${ARDIX_INCLUDE_DIRS})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.ld.in
	${CMAKE_BINARY_DIR}/config.ld # see toolchain.cmake
)

target_sources(ardix_arch PRIVATE
	atom_get_put.S
	atom.c
	atomic.c
	entry.c
	interrupt.c
	irq_pend_sv.S
	irq_svc.S
	sched.c
	serial.c
	startup.c
	sys.c
	syscall.S
	watchdog.c
)

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
