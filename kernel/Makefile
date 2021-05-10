#
# This file is part of Ardix.
# Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.
#
# Ardix is non-violent software: you may only use, redistribute,
# and/or modify it under the terms of the CNPLv6+ as found in
# the LICENSE file in the source code root directory or at
# <https://git.pixie.town/thufie/CNPL>.
#
# Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
# permitted by applicable law.  See the CNPLv6+ for details.
#

ARDIX_KERNEL_PWD = $(PWD)/kernel

include $(ARDIX_KERNEL_PWD)/fs/Makefile
include $(ARDIX_KERNEL_PWD)/io/Makefile

ARDIX_SOURCES += \
	$(ARDIX_KERNEL_PWD)/device.c \
	$(ARDIX_KERNEL_PWD)/dma.c \
	$(ARDIX_KERNEL_PWD)/kent.c \
	$(ARDIX_KERNEL_PWD)/printk.c \
	$(ARDIX_KERNEL_PWD)/ringbuf.c \
	$(ARDIX_KERNEL_PWD)/sched.c \
	$(ARDIX_KERNEL_PWD)/serial.c \
	$(ARDIX_KERNEL_PWD)/syscall.c \
	$(ARDIX_KERNEL_PWD)/userspace.c
