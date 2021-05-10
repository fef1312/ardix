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

AVR_CC ?= $(shell which avr-gcc)
AVR_OBJCOPY ?= $(shell which avr-objcopy)
AVR_LD ?= $(shell which avr-ld)

ARM_CC ?= $(shell which arm-none-eabi-gcc)
ARM_LD ?= $(shell which arm-none-eabi-ld)
ARM_OBJCOPY ?= $(shell which arm-none-eabi-objcopy)

EXTRA_CFLAGS ?=
CFLAGS = $(EXTRA_CFLAGS)

CFLAGS += -g -nodefaultlibs -nostartfiles
CFLAGS += -I$(PWD)/include
CFLAGS += -DARCH=$(ARCH)
CFLAGS += -fno-builtin -std=gnu11

EXTRA_LDFLAGS ?=
LDFLAGS = $(EXTRA_LDFLAGS)

ifeq ($(ARCH), at91sam3x8e)
    CFLAGS += -mcpu=cortex-m3 -mthumb -mabi=aapcs -march=armv7-m -masm-syntax-unified
    CC = $(ARM_CC)
    LD = $(ARM_LD)
    OBJCOPY = $(ARM_OBJCOPY)
else
    CFLAGS += -mmcu=$(ARCH)
    LDFLAGS += -mmcu=$(ARCH)
    CC = $(AVR_CC)
    LD = $(AVR_LD)
    OBJCOPY = $(AVR_OBJCOPY)
endif

CFLAGS += -Wall \
	-Wno-sign-conversion \
	-Wstrict-prototypes \
	-Wredundant-decls \
	-Wnested-externs \
	-Wbad-function-cast \
	-Wshadow \
	-Wsign-compare \
	-Wunreachable-code \
	-Wwrite-strings \
	-Wconversion \
	-Waggregate-return \
	-Winline \
	-Wcast-align

ifdef DEBUG
    CFLAGS += -DDEBUG
endif

ARDIX_ASM_SOURCES =
ARDIX_SOURCES =

include arch/Makefile

include init/Makefile
include lib/Makefile
include kernel/Makefile

ARDIX_OBJS = $(ARDIX_SOURCES:.c=.o)
ARDIX_ASM_OBJS = $(ARDIX_ASM_SOURCES:.S=.o)

%.o: %.S | %.c
	$(CC) -c -Os $(CFLAGS) $<

ardix.elf: $(ARDIX_ASM_OBJS) $(ARDIX_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

ardix.hex: ardix.elf
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

ardix.bin: ardix.elf
	$(OBJCOPY) -O binary -R .eeprom $^ $@

clean:
	rm -f ardix.elf ardix.hex ardix.bin $(ARDIX_OBJS) $(ARDIX_ASM_OBJS)

config:
	./configure

all: ardix.hex ardix.bin
