#include "vm.h"
#include "util/bits.h"
#include <cstdint>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 32
#define DELIM " \t"

int32_t parse_opcode(char *opcode)
{
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
	else
		return -1;
}

int32_t parse_register(char *r_name)
{
	if (strcmp("R0", r_name))
		return R_R0;
	else if (strcmp("R1", r_name))
		return R_R1;
	else if (strcmp("R2", r_name))
		return R_R2;
	else if (strcmp("R3", r_name))
		return R_R3;
	else if (strcmp("R4", r_name))
		return R_R4;
	else if (strcmp("R5", r_name))
		return R_R5;
	else if (strcmp("R6", r_name))
		return R_R6;
	else if (strcmp("R7", r_name))
		return R_R7;
	else if (strcmp("PC", r_name))
		return R_PC;
	else if (strcmp("COND", r_name))
		return R_COND;
	else if (strcmp("COUNT", r_name))
		return R_COUNT;
	else
		return -1;
}

int32_t parse_constant(char *val)
{
}

void op_add(uint16_t *instr)
{
	char *dr_str = strtok(NULL, DELIM);
	char *sr1_str = strtok(NULL, DELIM);
	char *sr2_str = strtok(NULL, DELIM);

	int32_t dr = parse_register(dr_str);
	int32_t sr1 = parse_register(sr1_str);
	int32_t sr2 = parse_register(sr2_str);

	write_bits(instr, 9, 11, dr);
	write_bits(instr, 6, 8, sr1);

	if (-1 == sr2)
		write_bits(instr, 0, 4, )
}

uint16_t assemble_instruction(char *line)
{
	uint16_t instr = 0;
	char *op = strtok(line, "\t ");

	write_bits(&instr, 12, 15, parse_opcode(op));

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
	char line[64] = "hi world 3";
	char *toks[24][24];

	tokenize(*toks, line);

	for (int i = 0; i < 4; i++)
		printf("%s\n", *toks[i]);
	// preprocess
	// for lines
	//   translate
	//   emit
}
