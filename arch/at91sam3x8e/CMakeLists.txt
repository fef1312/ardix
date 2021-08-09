# See the end of this file for copyright and license terms.

add_library(ardix_arch STATIC)
target_link_libraries(ardix INTERFACE ardix_arch)

target_compile_options(ardix_arch PRIVATE ${ARDIX_COMPILE_OPTIONS})
target_include_directories(ardix_arch PRIVATE
	${ARDIX_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/arduino-cmsis/CMSIS-Atmel/CMSIS/Device/ATMEL/sam3xa/include
	${CMAKE_CURRENT_SOURCE_DIR}/../../contrib/cmsis/CMSIS/Core/Include
)

target_compile_definitions(ardix_arch PRIVATE ARDIX_ARCH)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config/config.ld.in
	${CMAKE_BINARY_DIR}/config.ld # see toolchain.cmake
)

target_sources(ardix_arch PRIVATE
	arch_init.c
	atom_get_put.S
	atom.c
	atomic.c
	do_switch.S
	entry.c
	handle_fault.c
	handle_fault.S
	handle_pend_sv.S
	handle_reset.c
	handle_svc.S
	leave.S
	mutex.S
	sched.c
	serial.c
	sys.c
	syscall.S
	vector_table.c
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
