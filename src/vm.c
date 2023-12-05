#include "vm.h"
#include "io.h"
#include "op.h"
#include "util.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <signal.h>

int main(int argc, const char *argv[])
{
	struct vm_state state;

	if (argc < 2) {
		/* show usage string */
		printf("lc3 [image-file1] ...\n");
		exit(2);
	}

	for (int j = 1; j < argc; ++j) {
		if (!read_image(&state, argv[j])) {
			printf("failed to load image: %s\n", argv[j]);
			exit(1);
		}
	}

	signal(SIGINT, handle_interrupt);
disable_input_buffering();

	/* since exactly one condition flag should be set at any given time, set the Z flag */
	state.reg[R_COND] = FL_ZRO;

	/* set the PC to starting position */
	/* 0x3000 is the default */
	enum { PC_START = 0x3000 };
	state.reg[R_PC] = PC_START;
	state.running = true;

	while (state.running) {
		/* FETCH */
		uint16_t instr = mem_read(&state, state.reg[R_PC]++);
		uint16_t op = instr >> 12;

		switch (op) {
		case OP_ADD:
			op_add(&state, instr);
			break;
		case OP_AND:
			op_and(&state, instr);
			break;
		case OP_NOT:
			op_not(&state, instr);
			break;
		case OP_BR:
			op_br(&state, instr);
			break;
		case OP_JMP:
			op_jmp(&state, instr);
			break;
		case OP_JSR:
			op_jsr(&state, instr);
			break;
		case OP_LD:
			op_ld(&state, instr);
			break;
		case OP_LDI:
			op_ldi(&state, instr);
			break;
		case OP_LDR:
			op_ldr(&state, instr);
			break;
		case OP_LEA:
			op_lea(&state, instr);
			break;
		case OP_ST:
			op_st(&state, instr);
			break;
		case OP_STI:
			op_sti(&state, instr);
			break;
		case OP_STR:
			op_str(&state, instr);
			break;
		case OP_TRAP:
			op_trap(&state, instr);
			break;
		case OP_RES:
		case OP_RTI:
		default:
			abort();
			break;
		}
	}
	restore_input_buffering();
}
