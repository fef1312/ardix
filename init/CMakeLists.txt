# See the end of this file for copyright and license terms.

add_library(ardix_init STATIC)
target_link_libraries(ardix INTERFACE ardix_init)

target_compile_options(ardix_init PRIVATE ${ARDIX_COMPILE_OPTIONS})
target_include_directories(ardix_init PRIVATE ${ARDIX_INCLUDE_DIRS})

target_sources(ardix_init PRIVATE
	main.c
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
