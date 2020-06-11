#
# Copyright (c) 2020 Felix Kopp <sandtler@sandtler.club>
#
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this list of
#    conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice, this list of
#    conditions and the following disclaimer in the documentation and/or other materials
#    provided with the distribution.
# 3. Neither the name of the copyright holder nor the names of its contributors may be
#    used to endorse or promote products derived from this software without specific prior
#    written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
# WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
CFLAGS += -fpack-struct -std=gnu11

EXTRA_LDFLAGS ?=
LDFLAGS = $(EXTRA_LDFLAGS)

ifeq ($(ARCH), at91sam3x8e)
    CFLAGS += -mcpu=cortex-m3 -mthumb -mabi=aapcs -march=armv7-m
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
