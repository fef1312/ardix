# The Ardix Kernel

This is the source tree of Ardix, a microkernel for various Arduino boards that implements some core
concepts of the Unix philosophy.  Please note that this project is not affiliated with or endorsed
by Arduino s.r.l. in any way.  Only ARM-based boards are supported at the moment (and probably ever
will be, because AVR MCUs just don't have enough horsepower).

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
- CMake >= 3.14
- A programmer (`bossac` recommended)
- A Unix-like shell (sorry Microsoft lackeys, you can use the Windows Subsystem for Linux for
  compiling but probably not for flashing)

### Configuration

Configuration is done with the standard CMake config system.  See `options.cmake` for a full list
of available configuration variables.  In the simplest case, you will only need to specify the
target architecture (`ARCH`) to get a basic build.  The following architectures are supported:

- `at91sam3x8e` (Arduino Due)

### Build

To build the EEPROM image, execute the following command.
Pass any other configuration options you want to set to the first command or use `cmake-gui(1)`.

```shell
# Replace <target> with one of the target architectures from the list above
# This will default to at91sam3x8e (Arduino Due)
cmake -DARCH=<arch> -B build -S .
cmake --build build
```

This will create `ardix.bin` in the `build` directory, which can be passed to `bossac` for flashing.
If you are using an Arduino DUE (at91sam3x8e), make sure to connect the USB cable to the programmer
port (the one closer to the power jack).

```shell
# Replace <tty> with the name of the tty device in /dev
# that is connected to your Arduino
bossac -e -w -v -b -a --port=<tty> build/ardix.bin
```

Please refer to `bossac --help` for more information on how to use it.
