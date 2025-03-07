# aapen-term

Aapen-term is a Pi Pico based dumb terminal. While the orignal motivation was to build
a terminal to interact with the AApen operating system, Aapen-term is a more or less
generic UART based dumb terminal.

Aapen-term consists of two main parts:

* There is board, based on the Raspberry Pi Pico 2040 which had connections for a UART, a VGA monitor and a PS2 keyboard.
* And there is the firmware that runs on the board.

## Building the firmware

1. Make a build directory: `firmware/build`.
2. Cd to the `build` directory and run `cmake ..`.
3. Either in the `build` directory or in `firmware` you can now run `make`.

At the end of the build you will have three different versions of
the firmware:

* `firmware/build/term.uf2` - This is the actual termina firmware.
* `firmware/build/test_ps2.uf2` - This is a simple test firmware that will print a message for every keystroke.
* `firmware/build/test_vga.uf2` - This is a simple test firmware that print some text on the VGA screen.
