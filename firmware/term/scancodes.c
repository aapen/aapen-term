#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include <ctype.h>

static uint32_t caps_lock = 0;
static uint32_t key_up = 0;
static uint32_t control = 0;
static uint32_t shift = 0;
static uint32_t shift_lock = 0;
static uint32_t extended = 0;


static uint16_t toupper_digit(uint16_t digit) {
  uint16_t ch = '?';

  switch(digit) {
    case '1': 
      ch = '!';
      break;
    case '2': 
      ch = '@';
      break;
    case '3': 
      ch = '#';
      break;
    case '4': 
      ch = '$';
      break;
    case '5': 
      ch = '%';
      break;
    case '6': 
      ch = '^';
      break;
    case '7': 
      ch = '&';
      break;
    case '8': 
      ch = '*';
      break;
    case '9': 
      ch = '(';
      break;
    case '0': 
      ch = ')';
      break;
  }
  return ch;
}

static uint16_t toupper_punct(uint16_t punct) {
  uint16_t ch = '?';

  switch(punct) {
    case '`': 
      ch = '~';
      break;
    case '-': 
      ch = '_';
      break;
    case '=': 
      ch = '+';
      break;
    case '[': 
      ch = '{';
      break;
    case ']': 
      ch = '}';
      break;
    case '\\': 
      ch = '|';
      break;
    case ';': 
      ch = ':';
      break;
    case '\'': 
      ch = '"';
      break;
    case ',': 
      ch = '<';
      break;
    case '.': 
      ch = '>';
      break;
    case '/': 
      ch = '?';
      break;
  }
  return ch;
}

static uint16_t translate_scancode(uint16_t scancode) {
  uint16_t ch = 0;
  int upcase = shift ^ shift_lock;

  switch(scancode) {
    case 0x0e: 
      ch = '`';
      break;
    case 0x16: 
      ch = '1';
      break;
    case 0x1e: 
      ch = '2';
      break;
    case 0x26: 
      ch = '3';
      break;
    case 0x25: 
      ch = '4';
      break;
    case 0x2e: 
      ch = '5';
      break;
    case 0x36: 
      ch = '6';
      break;
    case 0x3d: 
      ch = '7';
      break;
    case 0x3e: 
      ch = '8';
      break;
    case 0x46: 
      ch = '9';
      break;
    case 0x45: 
      ch = '0';
      break;
    case 0x4e: 
      ch = '-';
      break;
    case 0x55: 
      ch = '=';
      break;
    case 0x66: 
      ch = '\b';
      break;
    case 0x0d: 
      ch = '\t';
      break;
    case 0x15: 
      ch = 'q';
      break;
    case 0x1d: 
      ch = 'w';
      break;
    case 0x24: 
      ch ='e';
      break;
    case 0x2d: 
      ch = 'r';
      break;
    case 0x2c: 
      ch = 't';
      break;
    case 0x3c: 
      ch = 'u';
      break;
    case 0x35: 
      ch = 'y';
      break;
    case 0x43: 
      ch = 'i';
      break;
    case 0x44: 
      ch = 'o';
      break;
    case 0x4d: 
      ch = 'p';
      break;
    case 0x54: 
      ch = '[';
      break;
    case 0x5b: 
      ch = ']';
      break;
    case 0x5d: 
      ch = '\\';
      break;
    case 0x76: 
      ch = 0x1b;
      break;
    case 0x1c: 
      ch = 'a';
      break;
    case 0x1b: 
      ch = 's';
      break;
    case 0x23: 
      ch ='d';
      break;
    case 0x2b: 
      ch ='f';
      break;
    case 0x34: 
      ch ='g';
      break;
    case 0x33: 
      ch = 'h';
      break;
    case 0x3b:
      ch = 'j';
      break;
    case 0x42: 
      ch = 'k';
      break;
    case 0x4B: 
      ch = 'l';
      break;
    case 0x4c: 
      ch = ';';
      break;
    case 0x52: 
      ch = '\'';
      break;
    case 0x5a: 
      ch = '\r';
      break;
    case 0x1a: 
      ch = 'z';
      break;
    case 0x22: 
      ch = 'x';
      break;
    case 0x21: 
      ch ='c';
      break;
    case 0x2a: 
      ch = 'v';
      break;
    case 0x32: 
      ch = 'b';
      break;
    case 0x31: 
      ch = 'n';
      break;
    case 0x3a: 
      ch = 'm';
      break;
    case 0x41: 
      ch = ',';
      break;
    case 0x49: 
      ch = '.';
      break;
    case 0x4a: 
      ch = '/';
      break;
    case 0x29: 
      ch = ' ';
      break;
  }
  printf("process scan code returns %x\n", ch);
  return ch;
}


int32_t process_key_event(uint16_t scancode) {
  uint32_t result = 0;

  if (scancode == 0xe0) {
    return -1;
  }

  if (key_up) {
    if (scancode == 0x14) {
      control = 0;
      printf("control UP\n");
    } else if (scancode == 0x12 || scancode == 0x59) {
      shift = 0;
    } else if (scancode == 0x3a) {
      shift_lock = 0;
    }
    key_up = 0;
    return -1;
  }

  if (scancode == 0xf0) { 
    key_up = 1;
    return -1;
  } else if (scancode == 0x14) {
    control = 1;
    key_up = 0;
    printf("control Down\n");
    return -1;
  } else if (scancode == 0x12 || scancode == 0x59) { 
    shift = 1;
    key_up = 0;
    return -1;
  } else if (scancode == 0x58) {
    shift_lock = shift_lock ? 0 : 1;
    key_up = 0;
    return -1;
  }

  key_up = 0;

  uint32_t ch = translate_scancode(scancode);
  printf("trans scan code: %x\n", ch);

  if (isalpha(ch)) {
    if (control) {
      return ch - 'a';
    } else if (shift ^ shift_lock) {
      return toupper(ch);
    }
  } else if (isdigit(ch)) {
    if (shift) {
      return toupper_digit(ch);
    }
  } else if (ispunct(ch)) {
    if (shift) {
      return toupper_punct(ch);
    }
  }

  return ch;
}
