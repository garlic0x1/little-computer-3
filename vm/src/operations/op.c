#include "op.h"
#include <stdint.h>
#include <stdlib.h>
#include "../util/bits.h"

/*
** NOTE
** the spec for operations can be found here:
** https://www.jmeiners.com/lc3-vm/supplies/lc3-isa.pdf
*/

/*
** Add:
** takes two numbers, adds them together, and stores the result in a register.
** If bit [5] is 0, the second source operand is obtained from SR2. If bit [5] is 1,
** the second source operand is obtained by sign-extending the imm5 field to 16 bits.
** In both cases, the second source operand is added to the contents of SR1 and the
** result stored in DR. The condition codes are set, based on whether the result is
** negative, zero, or positive.
*/
void op_add_(struct vm_state *state, uint16_t instr)
{
	// destination register
	uint16_t r0 = (instr >> 9) & 0x7;
	// first operand
	uint16_t r1 = (instr >> 6) & 0x7;
	// immediate mode?
	uint16_t imm_flag = (instr >> 5) & 0x1;

	if (imm_flag)
		state->reg[r0] = state->reg[r1] + sign_extend(instr & 0x1f, 5);
	else
		state->reg[r0] = state->reg[r1] + state->reg[instr & 0x7];

	update_flags(state, r0);
}

void op_add(struct vm_state *state, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t sr1 = extract_bits(instr, 6, 8);
	uint16_t imm_flag = extract_bits(instr, 5, 5);

	if (imm_flag)
		state->reg[dr] = state->reg[sr1] + extract_bits(instr, 0, 4);
	else
		state->reg[dr] = state->reg[sr1] + state->reg[extract_bits(instr, 0, 2)];

	update_flags(state, dr);
}

/*
** And:
** takes two numbers, applies bitwise and, and stores the result in a register.
** If bit [5] is 0, the second source operand is obtained from SR2. If bit [5] is 1,
** the second source operand is obtained by sign-extending the imm5 field to 16
** bits. In either case, the second source operand and the contents of SR1 are bit-
** wise ANDed, and the result stored in DR. The condition codes are set, based on
** whether the binary value produced, taken as a 2’s complement integer, is negative,
** zero, or positive.
*/
void op_and(struct vm_state *state, uint16_t instr)
{
	// destination register
	uint16_t r0 = (instr >> 9) & 0x7;
	// first operand
	uint16_t r1 = (instr >> 6) & 0x7;
	// immediate mode?
	uint16_t imm_flag = (instr >> 5) & 0x1;

	if (imm_flag)
		state->reg[r0] = state->reg[r1] & sign_extend(instr & 0x1f, 5);
	else
		state->reg[r0] = state->reg[r1] & state->reg[instr & 0x7];

	update_flags(state, r0);
}

/*
** Branch:
** The condition codes specified by the state of bits [11:9] are tested. If bit [11] is
** set, N is tested; if bit [11] is clear, N is not tested. If bit [10] is set, Z is
** tested, etc.  If any of the condition codes tested is set, the program branches to
** the location specified by adding the sign-extended PCoffset9 field to the
** incremented PC.
 */
void op_br(struct vm_state *state, uint16_t instr)
{
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	uint16_t cond_flag = (instr >> 9) & 0x7;

	if (cond_flag & state->reg[R_COND])
		state->reg[R_PC] += pc_offset;
}

/*
** Return and Jump:
** The program unconditionally jumps to the location specified by the contents of
** the base register. Bits [8:6] identify the base register.
*/
void op_jmp(struct vm_state *state, uint16_t instr)
{
	uint16_t r1 = (instr >> 6) & 0x7;
	state->reg[R_PC] = state->reg[r1];
}

/*
** Jump to Subroutine:
** First, the incremented PC is saved in R7. This is the linkage back to the calling
** routine. Then the PC is loaded with the address of the first instruction of the
** subroutine, causing an unconditional jump to that address. The address of the
** subroutine is obtained from the base register (if bit [11] is 0), or the address is
** computed by sign-extending bits [10:0] and adding this value to the incremented
** PC (if bit [11] is 1)
*/
void op_jsr(struct vm_state *state, uint16_t instr)
{
	uint16_t long_flag = (instr >> 11) & 0x1;
	state->reg[R_R7] = state->reg[R_PC];

	if (long_flag)
		state->reg[R_PC] += sign_extend(instr & 0x7ff, 11);
	else
		state->reg[R_PC] = (instr >> 6) & 0x7;
}

/*
** Load:
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. The contents of memory at this address are loaded
** into DR. The condition codes are set, based on whether the value loaded is
** negative, zero, or positive.
*/
void op_ld(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	state->reg[r0] = mem_read(state, state->reg[R_PC] + pc_offset);
	update_flags(state, r0);
}

/*
** Load Indirect:
** load a value from a location in memory into a register.
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. What is stored in memory at this address is the
** address of the data to be loaded into DR. The condition codes are set, based on
** whether the value loaded is negative, zero, or positive.
*/
void op_ldi(struct vm_state *state, uint16_t instr)
{
	// destination register
	uint16_t r0 = (instr >> 9) & 0x7;
	/* PCoffset 9*/
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	/* add offset to pc, and read memory */
	state->reg[r0] =
		mem_read(state, mem_read(state, state->reg[R_PC] + pc_offset));
	update_flags(state, r0);
}

/*
** Load Base + Offset:
** An address is computed by sign-extending bits [5:0] to 16 bits and adding this
** value to the contents of the register specified by bits [8:6]. The contents of
** memory at this address are loaded into DR. The condition codes are set, based
** on whether the value loaded is negative, zero, or positive.
*/
void op_ldr(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t r1 = (instr >> 6) & 0x7;
	uint16_t offset = sign_extend(instr & 0x3F, 6);
	state->reg[r0] = mem_read(state, state->reg[r1] + offset);
	update_flags(state, r0);
}

/*
** Load Effective Address:
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. This address is loaded into DR.‡ The condition
** codes are set, based on whether the value loaded is negative, zero, or positive
*/
void op_lea(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	state->reg[r0] = state->reg[R_PC] + pc_offset;
	update_flags(state, r0);
}

/*
** Not:
** The bit-wise complement of the contents of SR is stored in DR.
** The condition codes are set, based on whether the binary value produced,
** taken as a 2’s complement integer, is negative, zero, or positive.
*/
void op_not(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t r1 = (instr >> 6) & 0x7;

	state->reg[r0] = ~state->reg[r1];
	update_flags(state, r0);
}

/*
** Return from Subroutine:
** The PC is loaded with the value in R7.
** This causes a return from a previous JSR instruction.
*/
void op_ret(struct vm_state *state, uint16_t instr)
{
}

/*
** TODO
** Return from Interrupt:
** If the processor is running in Supervisor mode, the top two elements on the
** Supervisor Stack are popped and loaded into PC, PSR. If the processor is running
** in User mode, a privilege mode violation exception occurs.
*/
void op_rti(struct vm_state *state, uint16_t instr)
{
	abort();
}

/*
** Store:
** The contents of the register specified by SR are stored in the memory location
** whose address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC.
*/
void op_st(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	mem_write(state, state->reg[R_PC] + pc_offset, state->reg[r0]);
}

/*
** Store Indirect:
** The contents of the register specified by SR are stored in the memory location
** whose address is obtained as follows: Bits [8:0] are sign-extended to 16 bits and
** added to the incremented PC. What is in memory at this address is the address of
** the location to which the data in SR is stored.
 */
void op_sti(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
	mem_write(state, mem_read(state, state->reg[R_PC] + pc_offset),
		  state->reg[r0]);
}

/*
** Store Base + Offset:
** The contents of the register specified by SR are stored in the memory location
** whose address is computed by sign-extending bits [5:0] to 16 bits and adding this
** value to the contents of the register specified by bits [8:6].
*/
void op_str(struct vm_state *state, uint16_t instr)
{
	uint16_t r0 = (instr >> 9) & 0x7;
	uint16_t r1 = (instr >> 6) & 0x7;
	uint16_t offset = sign_extend(instr & 0x3F, 6);
	mem_write(state, state->reg[r1] + offset, state->reg[r0]);
}

/*
** TODO
*/
void op_res(struct vm_state *state, uint16_t instr)
{
	abort();
}

/*
** System Call:
** First R7 is loaded with the incremented PC. (This enables a return to the
** instruction physically following the TRAP instruction in the original program
** after the service routine has completed execution.) Then the PC is loaded
** with the starting address of the system call specified by trapvector8.
** The starting address is contained in the memory location whose address is
** obtained by zero-extending trapvector8 to 16 bits.
*/
void op_trap(struct vm_state *state, uint16_t instr)
{
	state->reg[R_R7] = state->reg[R_PC];
	trap(state, instr & 0xff);
}

// lookup operation to apply to instruction
void apply_op(struct vm_state *state, uint16_t op, uint16_t instr)
{
	switch (op) {
	case OP_ADD:
		op_add(state, instr);
		break;
	case OP_AND:
		op_and(state, instr);
		break;
	case OP_NOT:
		op_not(state, instr);
		break;
	case OP_BR:
		op_br(state, instr);
		break;
	case OP_JMP:
		op_jmp(state, instr);
		break;
	case OP_JSR:
		op_jsr(state, instr);
		break;
	case OP_LD:
		op_ld(state, instr);
		break;
	case OP_LDI:
		op_ldi(state, instr);
		break;
	case OP_LDR:
		op_ldr(state, instr);
		break;
	case OP_LEA:
		op_lea(state, instr);
		break;
	case OP_ST:
		op_st(state, instr);
		break;
	case OP_STI:
		op_sti(state, instr);
		break;
	case OP_STR:
		op_str(state, instr);
		break;
	case OP_TRAP:
		op_trap(state, instr);
		break;
	case OP_RES:
	case OP_RTI:
	default:
		abort();
		break;
	}
}
