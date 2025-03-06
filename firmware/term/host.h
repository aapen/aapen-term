#pragma once

void host_init();
void host_puts(char *s);
void host_putc(char ch);
bool host_data_available();
void host_on_recv(void (*handler)());
int16_t host_getc();
