#include "vga16_graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "string.h"

int main(){

  stdio_init_all() ;
  sleep_ms(900);
  printf("\nVGA Test Program\n");

  // Set up the screen.
  initVGA() ;
  resetScreen();
  setTextColor(WHITE) ;
  setBGColor(LIGHT_BLUE) ;
  sleep_ms(900);
  for(;;) {
    clearScreen();
    for (int i = 0; i<9; i++) {
      setTextSize(i+1);
      writeString("VGA output\n");
      sleep_ms(2000);
    }
    printf("Completed drawing one cycle\n");
    setTextPosition(0, 0);
  }
}
