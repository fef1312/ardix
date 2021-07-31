# See the end of this file for copyright and license terms.

cmake_minimum_required(VERSION 3.14.0)

set(ARCH "at91sam3x8e" CACHE STRING "Target architecture")
include(arch/${ARCH}/toolchain.cmake)

project(ardix VERSION 0.1.0 LANGUAGES C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    option(DEBUG "Enable debug features" ON)
endif()

include(options.cmake)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	execute_process(
		COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
		OUTPUT_VARIABLE ardix_GIT_REVISION
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	if (NOT "${ardix_GIT_REVISION}" STREQUAL "")
		set(ardix_VERSION_SUFFIX "-${ardix_GIT_REVISION}")
	endif()
endif()

add_library(ardix INTERFACE)
set(ARDIX_INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/arch/${ARCH}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
)
set(ARDIX_COMPILE_OPTIONS
	-nodefaultlibs
	-nostartfiles
	-fno-builtin
	-Wall
	-Wno-sign-conversion
	-Wstrict-prototypes
	-Wredundant-decls
	-Wnested-externs
	-Wbad-function-cast
	-Wshadow
	-Wsign-compare
	-Wunreachable-code
	-Wwrite-strings
	-Wconversion
	-Waggregate-return
	-Winline
	-Wcast-align
)

configure_file(
	${CMAKE_SOURCE_DIR}/include/config.h.in
	${CMAKE_BINARY_DIR}/include/config.h
)

# this must be included before any other subdirectories because
# it updates ARDIX_INCLUDE_DIRS and ARDIX_COMPILE_OPTIONS
add_subdirectory(arch)

add_subdirectory(init)
add_subdirectory(kernel)
add_subdirectory(lib)

target_include_directories(ardix INTERFACE ${ARDIX_INCLUDE_DIRS})
target_compile_options(ardix INTERFACE ${ARDIX_COMPILE_OPTIONS})

add_custom_target(ardix.elf DEPENDS ardix)
add_custom_command(
	TARGET ardix.elf
	COMMAND ${CMAKE_LINKER}
	ARGS ${ARDIX_LINKER_FLAGS} -o ardix.elf
		$<TARGET_FILE:ardix_arch>
		$<TARGET_FILE:ardix_init>
		$<TARGET_FILE:ardix_kernel>
		$<TARGET_FILE:ardix_kernel_fs>
		$<TARGET_FILE:ardix_lib>
)

add_custom_target(ardix.bin ALL DEPENDS ardix.elf)
add_custom_command(
	TARGET ardix.bin
	COMMAND ${CMAKE_OBJCOPY}
	ARGS -O binary -R .eeprom ardix.elf ardix.bin
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
