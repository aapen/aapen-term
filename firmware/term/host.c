#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200
//#define BAUD_RATE 9600


// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define DATA_BITS 8
#define STOP_BITS 1
//#define PARITY    UART_PARITY_EVEN
#define PARITY    UART_PARITY_NONE


void host_init() {
  // Set up our UART with the required speed.
  uart_init(UART_ID, BAUD_RATE);
  uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
  uart_set_fifo_enabled(UART_ID, 1);

  // Set the TX and RX pins by using the function select on the GPIO
  // Set datasheet for more information on function select
  gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
  gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
}

void host_on_recv(void (*handler)()) {
  // And set up and enable the interrupt handlers
  irq_set_exclusive_handler(UART0_IRQ, handler);
  irq_set_enabled(UART0_IRQ, true);

  // Now enable the UART to send interrupts - RX only
  uart_set_irq_enables(UART_ID, true, false);
}

void host_putc(char ch) {
  // Use some the various UART functions to send out data
  // In a default system, printf will also output via the default UART

  // Send out a character without any conversions
  uart_putc_raw(UART_ID, ch);
}

void host_puts(char* s) {
  for(int i=0; s[i]; i++) {
    host_putc(s[i]);
  }
}

bool host_data_available() {
  return uart_is_readable(UART_ID);
}

int32_t host_getc() {
  if (host_data_available()) {
    char ch = uart_getc(UART_ID);
    //printf("data from host! %x, %c\n", ch, ch);
    return ch;
  }
  return -1;
}


