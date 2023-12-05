#ifndef UTIL_H_
#define UTIL_H_

#include "vm.h"
#include <stdio.h>
#include <stdint.h>

uint16_t swap16(uint16_t x);
void update_flags(struct vm_state *state, uint16_t r);
int read_image(struct vm_state *state, const char *image_path);
uint16_t mem_read(struct vm_state *state, uint16_t address);
void mem_write(struct vm_state *state, uint16_t address, uint16_t val);


#endif // UTIL_H_
