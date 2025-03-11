#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"

#include "hardware/gpio.h"
#include "ps2.h"
#include "string.h"


void pulse(pin, n) {
    for(int j=0; j<n; j++) {
        gpio_put(pin, 1);
        sleep_ms(2);
        gpio_put(pin, 0);
        sleep_ms(1);
    }
}

/* Pulse gpio pins 16-21 with unique patterns. */ 

const int8_t start_pin = 16;
const int8_t end_pin = 21;

int main(){

  stdio_init_all() ;
  sleep_ms(1000);
  printf("\nVGA Pin test program\n\r");

  for(int i=start_pin; i<=end_pin; i++) {
     gpio_init(i); 
     gpio_set_dir(true);
     gpio_put(p, 0);
  }

  for(;;) {
    printf("loop\n");
    for(int i=start_pin; i<=end_pin; i++) {
        hi_digit = i / 10;
        low_digit = i % 10;
        pulse(i, hi_digit);
        sleep_ms(4);
        pulse(i, low_digit);
        sleep_ms(8)
    }
    sleep_ms(1000);
  }

}
