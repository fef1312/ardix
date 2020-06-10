# The Ardix Kernel

This is the source tree of Ardix, a microkernel for various Arduino boards that implements some core
concepts of the Unix philosophy.  Please note that this project is not affiliated with or endorsed
by Arduino s.r.l. in any way.

## License

Ardix is distributed under the terms of the BSD 3-clause license.
See the `LICENSE` file for details.

## Compiling

### Prerequisites

- The AVR tools, including
    * `avr-gcc`
    * `avr-ld`
    * `avr-objcopy`
- GNU `make`
- A Unix-like shell (sorry Microsoft lackeys, you can use the Windows Subsystem for Linux for
  compiling but not for flashing)

### Configuration

Right now, you have to manually define environment variables:

- `AVR_CC`: Full path to `avr-gcc`.  If unset, we will search for it in your PATH.
- `AVR_LD`: Full path to `avr-ld`.  If unset, we will search for it in your PATH.
- `AVR_OBJCOPY`: Full path to `avr-objcopy`.  If unset, we will search for it in your PATH.
- `ARCH`: Codename for the target architecture.  This is mandatory.
  The following architectures are currently supported:
    * `at91sam3x8e` (Arduino Due)
- `EXTRA_CFLAGS`: Any additional arguments that should be passed to the compiler.
- `EXTRA_LDFLAGS`: Any additional arguments that should be passed to the linker.

### Build

To build the EEPROM, execute the following command:

```shell
# Replace <target> with one of the target architectures from the list above
ARCH='<target>' make ardix.hex
```

This will create the `ardix.hex` file, which can be passed to `avrdude` for flashing:

```shell
avrdude -vvv -C '<config>' -p '<target>' -c '<programmer-id>' -P '<port>' -b '<baudrate>' -D -U flash:w:ardix.hex
```

Please refer to `man avrdude` for more information on how to use it.
