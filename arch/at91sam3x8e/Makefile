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
