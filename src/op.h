#ifndef OP_H_
#define OP_H_

#include <stdint.h>
#include "vm.h"

void op_add(struct vm_state *state, uint16_t instr);
void op_and(struct vm_state *state, uint16_t instr);
void op_br(struct vm_state *state, uint16_t instr);
void op_jmp(struct vm_state *state, uint16_t instr);
void op_jsr(struct vm_state *state, uint16_t instr);
void op_ld(struct vm_state *state, uint16_t instr);
void op_ldi(struct vm_state *state, uint16_t instr);
void op_ldr(struct vm_state *state, uint16_t instr);
void op_lea(struct vm_state *state, uint16_t instr);
void op_not(struct vm_state *state, uint16_t instr);
void op_ret(struct vm_state *state, uint16_t instr);
void op_rti(struct vm_state *state, uint16_t instr);
void op_st(struct vm_state *state, uint16_t instr);
void op_sti(struct vm_state *state, uint16_t instr);
void op_str(struct vm_state *state, uint16_t instr);
void op_res(struct vm_state *state, uint16_t instr);
void op_trap(struct vm_state *state, uint16_t instr);

uint16_t sign_extend(uint16_t x, int bit_count);

#endif // OP_H_
