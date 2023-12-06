#ifndef VM_H_
#define VM_H_

#include <stdint.h>

#define MEMORY_MAX (1 << 16)

// register names
enum {
	R_R0 = 0,
	R_R1,
	R_R2,
	R_R3,
	R_R4,
	R_R5,
	R_R6,
	R_R7,
	R_PC, /* program counter */
	R_COND,
	R_COUNT
};

// condition register states
enum {
	FL_POS = 1 << 0, /* P */
	FL_ZRO = 1 << 1, /* Z */
	FL_NEG = 1 << 2, /* N */
};

// memory mapped registers
enum {
	MR_KBSR = 0xFE00, /* keyboard status */
	MR_KBDR = 0xFE02 /* keyboard data */
};

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

#endif // VM_H_
