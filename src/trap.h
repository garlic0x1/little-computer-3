#ifndef TRAP_H_
#define TRAP_H_

#include <stdint.h>
#include "vm.h"

enum {
	TRAP_GETC = 0x20, /* get character from keyboard, not echoed */
	TRAP_OUT = 0x21, /* output a character */
	TRAP_PUTS = 0x22, /* output a word string */
	TRAP_IN = 0x23, /* get character from keyboard, echoed */
	TRAP_PUTSP = 0x24, /* output a byte string */
	TRAP_HALT = 0x25 /* halt the program */
};

void trap(struct vm_state *state, uint16_t trap_code);

#endif // TRAP_H_
