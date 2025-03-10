# aapen-term

Aapen-term is a Pi Pico based dumb terminal. While the orignal motivation was to build
a terminal to interact with the AApen operating system, Aapen-term is a more or less
generic UART based dumb terminal.

Aapen-term consists of two main parts:

* There is board, based on the Raspberry Pi Pico 2040 which had connections for a UART, a VGA monitor and a PS2 keyboard.
* And there is the firmware that runs on the board.

Note that the VGA code is based on the original by Hunter Adams (vha3@cornell.edu),
which can be found [on github](https://github.com/vha3/Hunter-Adams-RP2040-Demos/tree/master/VGA_Graphics).

## Building the firmware

1. Make a build directory: `firmware/build`.
2. Cd to the `build` directory and run `cmake ..`.
3. Either in the `build` directory or in `firmware` you can now run `make`.

At the end of the build you will have three different versions of
the firmware:

* `firmware/build/term.uf2` - This is the actual terminal firmware.
* `firmware/build/test_ps2.uf2` - This is a simple test firmware that will print a message for every keystroke.
* `firmware/build/test_vga.uf2` - This is a simple test firmware that print some text on the VGA screen.

## Terminal protocol

The protocol implemented by the firmware is based on a simple stack oriented
set of commands.

With the exception of the escape character, any byte received from the host is simply echoed out to the screen.
The escape commands are:

* escape 1 - read the next byte and push it as a 16 bit number onto the stack
* escape 2 - read the next two bytes, turn them into a 16 bit number and push that onto the stack. Hi byte first.
* escape H - print hello on the screen
* escape P - pop the value off of the stack and print it on the screen
* escape l - toggle local echo
* escape D - drop a number from the stack
* escape r - pop color, h, w, y and x off of the stack and fill a rectangle.
* escape h - home the cursor
* escape p - pop x and y off of the stack and move the cursor to that character position.
* escape c - pop a number off of the stack and set the text color. Values 0-7.
* escape S - dump the stack onto the console
* escape X - reset the protocol handler
* escape T - display a VGA test screen.
