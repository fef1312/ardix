# See the end of this file for copyright and license terms.

add_library(ardix_kernel_fs STATIC)
target_link_libraries(ardix_kernel PUBLIC ardix_kernel_fs)

target_compile_options(ardix_kernel_fs PRIVATE ${ARDIX_COMPILE_OPTIONS})
target_include_directories(ardix_kernel_fs PRIVATE ${ARDIX_INCLUDE_DIRS})

target_sources(ardix_kernel_fs PRIVATE
	file.c
	read.c
	write.c
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
