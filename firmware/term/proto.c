#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "vga16_graphics.h"


const uint8_t ASCII_MASK = 0x80;

enum opcodes {OC_ERROR=0x80,      OC_HELLO= 0x81,  OC_PUSH8=0x82, OC_PUSH16=0x83,
              OC_CURSOR_TO=0x84,  OK_HOME=0x85,    OC_CHAR=0x86,    OC_DROP=0x87,
              OC_RECT=0x88,       OC_TEXT_COLOR=0x89, OC_STACK=0x8a, OC_HOME=0x8b,
	      OC_VGA_TEST=0x8c};


#define MAX_STACK 64
static uint16_t stack[MAX_STACK];
static uint16_t i_stack = 0;

static uint8_t opcode = OC_ERROR;
static uint8_t args[5];
static uint8_t arg_bytes_remaining = 0;
static uint8_t arg_bytes_read = 0;

static uint8_t get_arg_len(uint8_t opcode) {
  uint8_t result = 0;
  switch(opcode) {
    case OC_PUSH8:
      result = 1;
      break;
    case OC_PUSH16:
      result = 2;
      break;
  }
  return result;
}

static void dump_stack() {
  printf("stack:: %d\n", i_stack);
  for(int i=0; i < i_stack; i++) {
    printf("Stack %d: %hx\n",  i, stack[i]);
  }
}

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

static void process_instruction(uint8_t opcode, uint8_t *args) {
  uint16_t x, y, w, h, c;

  //printf("process ins: %x ", opcode);

  if ((opcode & ASCII_MASK) == 0) {
    //printf("ascii code\n");
    writeChar(opcode);
    return;
  }

  //printf("command\n");
  switch(opcode) {
    case OC_ERROR:
      printf("ERROR\n");
      writeChar('?');
      break;
     case OC_HELLO:
      clearScreen();
      writeString("hello\n");
      break;
    case OC_PUSH8:
      printf("push-8 %hhd\n", args[0]);
      stack[i_stack++] = (uint16_t)args[0];
      dump_stack();
      break;
    case OC_PUSH16:
      printf("push-16 %x %x\n", args[0], args[1]);
      stack[i_stack++] = (uint16_t)args[0] << 8 | (uint16_t)args[1];
      dump_stack();
      break;
    case OC_CHAR:
      writeChar(stack[--i_stack]);
      break;
    case OC_DROP:
      i_stack--;
      break;
    case OC_RECT:
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
    case OC_HOME:
      printf("home\n");
      setCursor(0, 0);
      break;
    case OC_CURSOR_TO:
      x = stack[--i_stack];
      y = stack[--i_stack];
      setTextPosition(x, y);
      break;
    case OC_TEXT_COLOR:
      setTextColor(stack[--i_stack]);
      break;
    case OC_STACK:
      dump_stack();
      break;
    case OC_VGA_TEST:
      vga_test();
      break;
    default:
      printf("%x???\n", opcode);
  }
}

void process_byte(uint8_t b) {
  //printf("process byte: %x %d %d\n", b, arg_bytes_remaining, arg_bytes_read);

  if (arg_bytes_remaining == 0) {
    opcode = b;
    arg_bytes_remaining = get_arg_len(opcode);
    arg_bytes_read = 0;
  } else {
    args[arg_bytes_read++] = b;
    arg_bytes_remaining--;
  }

  //printf("p2: %x %d %d\n", b, arg_bytes_remaining, arg_bytes_read);

  if(arg_bytes_remaining == 0) {
    process_instruction(opcode, args);
    arg_bytes_read = 0;
    opcode = OC_ERROR;
  }
}

