#include "trap.h"
#include "vm.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void trap_puts(struct vm_state *state, uint16_t trap_code)
{
	/* one char per word */
	uint16_t *c = state->memory + state->reg[R_R0];
	while (*c) {
		putc((char)*c, stdout);
		++c;
	}
	fflush(stdout);
}

void trap_getc(struct vm_state *state, uint16_t trap_code)
{
	/* read a single ASCII char */
	state->reg[R_R0] = (uint16_t)getchar();
	update_flags(state, R_R0);
}

void trap_out(struct vm_state *state, uint16_t trap_code)
{
	putc((char)state->reg[R_R0], stdout);
	fflush(stdout);
}

void trap_in(struct vm_state *state, uint16_t trap_code)
{
	printf("Enter a character: ");
	char c = getchar();
	putc(c, stdout);
	fflush(stdout);
	state->reg[R_R0] = (uint16_t)c;
	update_flags(R_R0);
}

void trap_putsp(struct vm_state *state, uint16_t trap_code)
{
	/* one char per byte (two bytes per word)
       here we need to swap back to
       big endian format */
	uint16_t *c = state->memory + state->reg[R_R0];
	while (*c) {
		char char1 = (*c) & 0xFF;
		putc(char1, stdout);
		char char2 = (*c) >> 8;
		if (char2)
			putc(char2, stdout);
		++c;
	}
	fflush(stdout);
}

void trap_halt(struct vm_state *state, uint16_t trap_code)
{
	puts("HALT");
	fflush(stdout);
	state->running = false;
}

void trap(struct vm_state *state, uint16_t trap_code)
{
	switch (trap_code) {
	case TRAP_GETC:
		break;
	case TRAP_OUT:
		break;
	case TRAP_PUTS:
		break;
	case TRAP_IN:
		break;
	case TRAP_PUTSP:
		break;
	case TRAP_HALT:
		break;
	}
}
