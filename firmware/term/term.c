#include "vga16_graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "ps2.h"
#include "scancodes.h"
#include "host.h"
#include "proto.h"

#include "pico/multicore.h"
#include "string.h"

static queue_t input_queue;

static const int32_t KB_MASK= 1 << 31;
static const int32_t HOST_MASK= 1 << 30;

static inline bool is_from_keyboard(uint32_t ch) {
  return ch & KB_MASK;
}

static inline bool is_from_host(uint32_t ch) {
  return ch & HOST_MASK;
}

static inline int32_t extract_data(uint32_t ch) {
  return ch & ~(KB_MASK | HOST_MASK);
}

static void on_host_rx() {
  while (host_data_available()) {
    uint32_t ch = host_getc();
    ch = ch | HOST_MASK;
    if (!queue_try_add(&input_queue, &ch)) {
      printf("host FIFO was full\n");
    }
  }
}

void kbd_handler(int32_t scan_code) {
  //printf("handler kb event char %x\n", scan_code);
  scan_code = scan_code | KB_MASK;
  //printf("handler kb event with mask %x\n", scan_code);
  if (!queue_try_add(&input_queue, &scan_code)) {
    printf("FIFO was full\n");
  }
}

// Runs on the second code, do nothing right now.

void core1_main() {
  printf("Core 1 main init\n");

  while(true) {
    printf("Core 1 main loop\n");
    sleep_ms(5000);
  }
}


// Main program for core0, the thing that is started on boot.

int main(){

  stdio_init_all() ;
  host_init();
  printf("\n\rCombo ps2 vga test program\n\r");
  sleep_ms(900);
  printf("\n\rCombo ps2 vga test program\n\r");

  queue_init(&input_queue, sizeof(uint32_t), 8192);

  // Set up the screen.
  initVGA() ;
  resetScreen();
  sleep_ms(900);
  writeString("** AApen Term **\n");
  writeString("Booting...");
  setTextSize(1);
  setTextColor(WHITE) ;
  setBGColor(LIGHT_BLUE) ;
  clearScreen();
  writeString("** AApen Term Up **\n");


  // Set up the ps2 keyboard.
  printf("setting up keyboard\n");
  kbd_init(kbd_handler, 500);

  //multicore_launch_core1(core1_main);

  // Set up uart to host.
  host_on_recv(on_host_rx);

  // And we are off.  
  writeString("Booted!\n");
  printf("Booted\n");

  for(;;) {
    int32_t data, event, ch;
    queue_remove_blocking(&input_queue, &event);
    data = extract_data(event);
    //printf("main: event: %x\n", data);
    if(is_from_keyboard(event)) {
      ch = process_key_event(data);
      //printf("main: from keyboard %d\n", ch);
      if (ch >= 0) {
        host_putc(ch);
        // writeChar(ch);     // Local echo
      }
    } else if (is_from_host(event)) {
      process_byte(data);
    } else {
      printf("Dont know what to do with event: %x\n", event);
    }

  }
}
