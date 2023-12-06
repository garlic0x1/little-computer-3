#ifndef UTIL_H_
#define UTIL_H_

#include "../machine/vm.h"
#include <stdio.h>
#include <stdint.h>

uint16_t swap16(uint16_t x);
void update_flags(struct vm_state *state, uint16_t r);
int read_image(struct vm_state *state, const char *image_path);
uint16_t mem_read(struct vm_state *state, uint16_t address);
void mem_write(struct vm_state *state, uint16_t address, uint16_t val);
uint16_t sign_extend(uint16_t x, int bit_count);
void load_bytecode(struct vm_state *state, char *filename);
void vm_init(struct vm_state *state);

#endif // UTIL_H_
