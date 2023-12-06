#ifndef OP_H_
#define OP_H_

#include "../machine/vm.h"
#include "../syscall/trap.h"
#include "../util/util.h"

#include <stdint.h>

// opcodes
enum {
	OP_BR = 0, /* branch */
	OP_ADD, /* add  */
	OP_LD, /* load */
	OP_ST, /* store */
	OP_JSR, /* jump register */
	OP_AND, /* bitwise and */
	OP_LDR, /* load register */
	OP_STR, /* store register */
	OP_RTI, /* unused */
	OP_NOT, /* bitwise not */
	OP_LDI, /* load indirect */
	OP_STI, /* store indirect */
	OP_JMP, /* jump */
	OP_RES, /* reserved (unused) */
	OP_LEA, /* load effective address */
	OP_TRAP /* execute trap */
};

void apply_op(struct vm_state *state, uint16_t op, uint16_t instr);

#endif // OP_H_
