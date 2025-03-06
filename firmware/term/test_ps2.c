#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"

#include "hardware/gpio.h"
#include "ps2.h"
#include "string.h"

void kbd_handler(int32_t scan_code) {
  printf("handler kb event char %x\n", scan_code);
}

/* Simply print out a message when we see a keystroke */

int main(){

  stdio_init_all() ;
  printf("\nPS2 test program\n\r");

  // Set up the ps2 keyboard.
  printf("setting up keyboard\n");
  kbd_init(kbd_handler, 500);

  for(;;) {
    sleep_ms(1000);
    printf("Idle loop\n");
  }
}
