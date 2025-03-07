#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "vga16_graphics.h"

typedef (void (*ByteHandler)(int32t));
static ByteHandler byte_handler = command_byte_handler;

static uint8_t stack_bytes_remaining = 0;
static uint16_t stack_temp = 0;

const ESCAPE = 0x1b;

#define MAX_STACK 64
static uint16_t stack[MAX_STACK];
static uint16_t i_stack = 0;

// Dump the stack.
static void dump_stack() {
  printf("stack:: %d\n", i_stack);
  for(int i=0; i < i_stack; i++) {
    printf("Stack %d: %hx\n",  i, stack[i]);
  }
}

// Run a vga test.
static void vga_test() {
  char buf[100];
  printf("vga test\n");
  int c = 0;
  for(int r=0; r < 66; r++) {
    setTextPosition(c++, r);
    sprintf(buf, "Row %d col %d----", r, c);
    writeString(buf);
  }

  setTextPosition(5, 50);
  writeString("***row 5 col 50");
}

// Handles the byte after we see an escape.
static void escape_byte_handler(uint8_t ch) {
  uint16_t x, y, w, h, c;

  printf("escape byte handler: %x ", ch);

  //printf("command\n");
  switch(ch) {
    case OC_ERROR:
      printf("ERROR\n");
      writeChar('?');
      break;
     case 'h':
      clearScreen();
      writeString("hello\n");
      break;
    case '1':
      printf("push-8\n");
      stack_bytes_remaining = 1;
      stack_byte_handler = stack_byte_handler;
      break;
    case '2':
      printf("push-16\n");
      stack_bytes_remaining = 2;
      stack_byte_handler = stack_byte_handler;
      break;
    case 'P':
      writeChar(stack[--i_stack]);
      break;
    case 'D':
      i_stack--;
      break;
    case 'r':
      printf("rect\n");
      dump_stack();
      c = stack[--i_stack];
      h = stack[--i_stack];
      w = stack[--i_stack];
      y = stack[--i_stack];
      x = stack[--i_stack];
      printf("rect %hx %hx %hx %hx %hx\n", x, y, w, h, c);
      fillRect(x, y, w, h, c);
      break;
    case 'h':
      printf("home\n");
      setCursor(0, 0);
      break;
    case 'p':
      x = stack[--i_stack];
      y = stack[--i_stack];
      setTextPosition(x, y);
      break;
    case 'c':
      setTextColor(stack[--i_stack]);
      break;
    case 'S':
      dump_stack();
    case 'X':
      printf("Protocol Reset!\n");
      stack_bytes_remaining = 0;
      i_stack = 0;
      byte_handler = ground_byte_handler;
      break;
    case 'T':
      vga_test();
      break;
    default:
      printf("%x???\n", ch);
  }
}

// Handles pushing values onto the stack after ESC 1 or ESC 2.
static void stack_byte_handler(uint8_t ch) {
  if (stack_bytes_remaining == 0) {
    printf("Error: stack byte handler called with no more bytes\n");
  } else {
    stack_temp = (stack_temp << 8) | ch;
    stack_bytes_remaining--;
  }

  if (stack_bytes_remaining == 0) {
    byte_handler = command_byte_handler;
  }
}

// Basic byte handler, deals with everything except the escape commands.
static void ground_byte_handler(uint8_t b) {
  if (b == ESCAPE) {
    byte_handler = escape_byte_handler;
  } else {
    writeChar(b);
  }
}

void process_byte(uint8_t b) {
  byte_handler(b);
}

