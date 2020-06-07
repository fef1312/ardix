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
AVR_LD ?= $(shell which avr-gcc)
AVR_OBJCOPY ?= $(shell which avr-objcopy)

EXTRA_CFLAGS ?=
CFLAGS = $(EXTRA_CFLAGS)

CFLAGS += -nostartfiles -nostdlib -nodefaultlibs -fno-builtin
CFLAGS += -I$(PWD)/include
CFLAGS += -DARCH=$(ARCH)
CFLAGS += -mmcu=$(ARCH) -fpack-struct -std=gnu11

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

EXTRA_LDFLAGS ?=
LDFLAGS = $(EXTRA_LDFLAGS)

LDFLAGS += \
	-mmcu=$(ARDIX_ARCH) \
	-nostdlib \
	-nodefaultlibs

ARDIX_SOURCES =
include init/Makefile
ARDIX_OBJS = $(ARDIX_SOURCES:.c=.o)

%.o: %.c
	$(AVR_CC) -c -Os -o $@ $(CFLAGS) $<

ardix.elf: $(ARDIX_OBJS)
	$(AVR_LD) $(LDFLAGS) -o $@ $^

ardix.hex: ardix.elf
	$(AVR_OBJCOPY) -O ihex -R .eeprom $^ $@

clean:
	rm -f ardix.elf ardix.hex $(shell find . -name '*.o')

all: ardix.hex
