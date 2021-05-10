# The Ardix Kernel

This is the source tree of Ardix, a microkernel for various Arduino boards that implements some core
concepts of the Unix philosophy.  Please note that this project is not affiliated with or endorsed
by Arduino s.r.l. in any way.  Only ARM-based boards are supported at the moment.

## License

Copyright (c) 2020, 2021 Felix Kopp <owo@fef.moe>.

Ardix is non-violent software: you may only use, redistribute,
and/or modify it under the terms of the CNPLv6+ as found in
the LICENSE file in the source code root directory or at
<https://git.pixie.town/thufie/CNPL>.

Ardix comes with ABSOLUTELY NO WARRANTY, to the extent
permitted by applicable law.  See the CNPLv6+ for details.

## Compiling

### Prerequisites

- The GNU toolchain, including
    * `arm-gcc`
    * `arm-ld`
    * `arm-objcopy`
- GNU `make`
- A programmer (`bossac` recommended)
- A Unix-like shell (sorry Microsoft lackeys, you can use the Windows Subsystem for Linux for
  compiling but probably not for flashing)

### Configuration

Right now, you have to manually define environment variables:

- `ARM_CC`: Full path to `arm-none-eabi-gcc`.  If unset, we will search for it in your PATH.
- `ARM_LD`: Full path to `arm-none-eabi-ld`.  If unset, we will search for it in your PATH.
- `ARM_OBJCOPY`: Full path to `arm-none-eabi-objcopy`.
  If unset, we will search for it in your PATH.
- `ARCH`: Codename for the target architecture.  This is mandatory.
  The following architectures are currently supported:
    * `at91sam3x8e` (Arduino Due)
- `EXTRA_CFLAGS`: Any additional arguments that should be passed to the compiler.
- `EXTRA_LDFLAGS`: Any additional arguments that should be passed to the linker.

### Build

To build the EEPROM image, execute the following command:

```shell
# Replace <target> with one of the target architectures from the list above
ARCH='<target>' make ardix.bin
```

This will create the `ardix.bin` file, which can be passed to `bossac` for flashing.
If you are using an Arduino DUE (at91sam3x8e), make sure to connect the USB cable to the programmer
port (the one closer to the power jack).

```shell
# Replace <tty> with the name of the tty device in /dev
# that is connected to your Arduino
bossac -e -w -v -b -a --port=<tty> ardix.bin
```

Please refer to `bossac --help` for more information on how to use it.
