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

ARDIX_ARCH_PWD = $(PWD)/arch/at91sam3x8e

ARDIX_SOURCES += \
	$(ARDIX_ARCH_PWD)/atom.c \
	$(ARDIX_ARCH_PWD)/atomic.c \
	$(ARDIX_ARCH_PWD)/entry.c \
	$(ARDIX_ARCH_PWD)/interrupt.c \
	$(ARDIX_ARCH_PWD)/sched.c \
	$(ARDIX_ARCH_PWD)/serial.c \
	$(ARDIX_ARCH_PWD)/startup.c \
	$(ARDIX_ARCH_PWD)/sys.c \
	$(ARDIX_ARCH_PWD)/watchdog.c

ARDIX_ASM_SOURCES += \
	$(ARDIX_ARCH_PWD)/irq_pend_sv.S \
	$(ARDIX_ARCH_PWD)/irq_svc.S \
	$(ARDIX_ARCH_PWD)/syscall.S

CFLAGS += \
	-DARCH_AT91SAM3X8E

LDFLAGS += \
	-T$(ARDIX_ARCH_PWD)/config.ld \
	-T$(ARDIX_ARCH_PWD)/flash.ld
