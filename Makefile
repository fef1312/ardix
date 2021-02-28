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
	-Wstrict-prototypes \
	-Wredundant-decls \
	-Wnested-externs -Wbad-function-cast \
	-Wshadow \
	-Wsign-compare -Wfloat-equal \
	-Wunreachable-code \
	-Wwrite-strings -Wconversion \
	-Waggregate-return -Winline -Wcast-align

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
