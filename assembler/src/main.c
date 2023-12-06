#include "vm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 32

uint16_t map_opcode(char* opcode) {
    if (strcmp("BR", opcode))
        return OP_BR;
    else if (strcmp("ADD", opcode))
        return OP_ADD;
    else if (strcmp("LD", opcode))
        return OP_LD;
    else if (strcmp("ST", opcode))
        return OP_ST;
    else if (strcmp("JSR", opcode))
        return OP_JSR;
    else if (strcmp("AND", opcode))
        return OP_AND;
    else if (strcmp("LDR", opcode))
        return OP_LDR;
    else if (strcmp("STR", opcode))
        return OP_STR;
    else if (strcmp("RTI", opcode))
        return OP_RTI;
    else if (strcmp("NOT", opcode))
        return OP_NOT;
    else if (strcmp("LDI", opcode))
        return OP_LDI;
    else if (strcmp("STI", opcode))
        return OP_STI;
    else if (strcmp("JMP", opcode))
        return OP_JMP;
    else if (strcmp("RES", opcode))
        return OP_RES;
    else if (strcmp("LEA", opcode))
        return OP_LEA;
    else if (strcmp("TRAP", opcode))
        return OP_TRAP;
    else exit(-1);
}

uint16_t assemble_instruction(char *line)
{
    char *op_tok = strtok(line, "\t ");
    uint16_t op = map_opcode(op_tok);

    // TODO

    return op;
}

void assemble_file(FILE *code)
{
	size_t size = MAXLINE;
	char *line[size];

	while (-1 != getline(line, &size, code)) {
	}
}

int main(int argc, char *argv[])
{
	// preprocess
	// for lines
	//   translate
	//   emit
}
