#include "util/io.h"
#include "util/util.h"
#include "operations/op.h"
#include "machine/vm.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// state lives at the bottom of the stack
	struct vm_state state;

	// make sure at least one object file is specified
	if (argc < 2) {
		printf("lc-3 [image-file] ...\n");
		exit(-1);
	}

	// load files from CLI args
	for (int argi = 1; argi < argc; argi++)
		load_bytecode(&state, argv[argi]);

	// prepare the state of the VM
	vm_init(&state);

	// main loop
	while (state.running) {
		uint16_t instr = mem_read(&state, state.reg[R_PC]++);
		uint16_t op = instr >> 12;

		/* printf("op: %x, instr: %.4x\n", op, instr); */

		// perform instruction
		apply_op(&state, op, instr);
	}

	// cleanup
	restore_input_buffering();
}
