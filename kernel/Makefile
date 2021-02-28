#
# This file is part of Ardix.
# Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.
#
# Ardix is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Ardix is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
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
