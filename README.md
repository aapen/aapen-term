# aapen-term

Aapen-term is a Pi Pico based dumb terminal. While the orignal motivation was to build
a terminal to interact with the AApen operating system, Aapen-term is a more or less
generic UART based dumb terminal.

Aapen-term consists of two main parts:

* There is board, based on the Raspberry Pi Pico 2040 which had connections for a UART, a VGA monitor and a PS2 keyboard.
* And there is the firmware that runs on the board.
