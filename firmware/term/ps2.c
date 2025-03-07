#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/clocks.h"

#define CLK_GPIO 26 // PS/2 clock
#define DAT_GPIO 27 // PS/2 data

static void pin_input_pu(uint8_t p) {
  gpio_init(p);
  gpio_set_dir(p, false);
  gpio_disable_pulls(p);
}

static void pin_output(uint8_t p) {
  gpio_init(p);
  gpio_set_drive_strength(p, GPIO_DRIVE_STRENGTH_4MA);
  gpio_set_dir(p, true);
}

static void pin_write(uint8_t p, uint8_t v) {
  gpio_put(p, v ? 1: 0);
}

static void setup_pins(uint8_t pinData, uint8_t pinClock)
{
  printf("new setup\n");

  // Set pins to output
  pin_output(pinClock);
  pin_output(pinData);

  // Release clock
  //pinMode(pinClock, INPUT_PULLUP);
  pin_input_pu(pinClock);

  printf("Waiting for clock low\n");
  while(gpio_get(pinClock) != 0);

  // Release data line (which keeps it high for the stop bit)
  //pinMode(pinData, INPUT_PULLUP);
  pin_input_pu(pinData);

  printf("Waiting for data low\n");

  //Wait for data low, which would be the ack from keyboard
  while(gpio_get(pinData) != 0);  

  printf("done setup\n");
}


static int bit_count = 0;
static int scan_code = 0;
static void (*kbd_handler)(int32_t);

void ps2_clock_callback(uint gpio, uint32_t events) {
  bool value = gpio_get(DAT_GPIO);
  //printf("GPIO %d->%d  %d ", gpio, value, bit_count);

  if (bit_count == 0) {
    if (value == 0) {
      //printf("start bit!\n");
      scan_code = 0;
      bit_count++;
    }
  } else if (bit_count < 9) {
    //printf("data!\n");
    scan_code |= (value << (bit_count-1));
    bit_count++;
  } else if (bit_count == 9) {
    //printf("parity ignored!\n");
    bit_count++;
  } else if (bit_count >= 10) {
    if (value != 1) {
      printf("bad stop bit!\n");
    } 
    //printf("  SCAN CODE: %x\n", scan_code);
    kbd_handler(scan_code);
    bit_count = 0;
    scan_code = 0;
  }
}


// Initialize the PS2 keyboard driver.
// * handler is a fn pointer, gets called with each scan code.
// * clear_time is ms to wait for the kb to settle down.
void kbd_init(void (*handler)(int32_t), int32_t clear_time) {
  // init KBD pins to input

  //printf("Event driven PS/2 \n");

  setup_pins(DAT_GPIO, CLK_GPIO);
  //printf("waiting for input to clear\n");
  sleep_ms(clear_time);
  kbd_handler = handler;
  gpio_set_irq_enabled_with_callback(CLK_GPIO, GPIO_IRQ_EDGE_FALL, true, &ps2_clock_callback);
  //printf("complete!\n");
}


