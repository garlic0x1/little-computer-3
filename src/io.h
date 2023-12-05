#ifndef IO_H_
#define IO_H_

#include <stdint.h>

void disable_input_buffering();
void restore_input_buffering();
uint16_t check_key();
void handle_interrupt(int signal);

#endif // IO_H_
