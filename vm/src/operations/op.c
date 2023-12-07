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
void op_add(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t sr1 = extract_bits(instr, 6, 8);
	uint16_t imm_flag = extract_bits(instr, 5, 5);

	if (imm_flag) {
		uint16_t imm5 = extend_bits(instr, 0, 4);
		vm->reg[dr] = vm->reg[sr1] + imm5;
	} else {
		uint16_t sr2 = extract_bits(instr, 0, 2);
		vm->reg[dr] = vm->reg[sr1] + vm->reg[sr2];
	}

	update_flags(vm, dr);
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
void op_and(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t sr1 = extract_bits(instr, 6, 8);
	uint16_t imm_flag = extract_bits(instr, 5, 5);

	if (imm_flag) {
		uint16_t imm5 = extend_bits(instr, 0, 4);
		vm->reg[dr] = vm->reg[sr1] & imm5;
	} else {
		uint16_t sr2 = extract_bits(instr, 0, 2);
		vm->reg[dr] = vm->reg[sr1] & vm->reg[sr2];
	}

	update_flags(vm, dr);
}

/*
** Branch:
** The condition codes specified by the state of bits [11:9] are tested. If bit [11] is
** set, N is tested; if bit [11] is clear, N is not tested. If bit [10] is set, Z is
** tested, etc.  If any of the condition codes tested is set, the program branches to
** the location specified by adding the sign-extended PCoffset9 field to the
** incremented PC.
 */
void op_br(struct vm_state *vm, uint16_t instr)
{
	uint16_t cond_flag = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);

	if (cond_flag & vm->reg[R_COND])
		vm->reg[R_PC] += pc_offset;
}

/*
** Return and Jump:
** The program unconditionally jumps to the location specified by the contents of
** the base register. Bits [8:6] identify the base register.
*/
void op_jmp(struct vm_state *vm, uint16_t instr)
{
	uint16_t base_r = extract_bits(instr, 6, 8);
	vm->reg[R_PC] = vm->reg[base_r];
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
void op_jsr(struct vm_state *vm, uint16_t instr)
{
	uint16_t long_flag = extract_bits(instr, 11, 11);
	vm->reg[R_R7] = vm->reg[R_PC];

	if (long_flag) {
		uint16_t pc_offset = extend_bits(instr, 0, 10);
		vm->reg[R_PC] += pc_offset;
	} else {
		uint16_t base_r = extract_bits(instr, 6, 8);
		vm->reg[R_PC] = base_r;
	}
}

/*
** Load:
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. The contents of memory at this address are loaded
** into DR. The condition codes are set, based on whether the value loaded is
** negative, zero, or positive.
*/
void op_ld(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);
	vm->reg[dr] = mem_read(vm, vm->reg[R_PC] + pc_offset);
	update_flags(vm, dr);
}

/*
** Load Indirect:
** load a value from a location in memory into a register.
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. What is stored in memory at this address is the
** address of the data to be loaded into DR. The condition codes are set, based on
** whether the value loaded is negative, zero, or positive.
*/
void op_ldi(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);
	vm->reg[dr] = mem_read(vm, mem_read(vm, vm->reg[R_PC] + pc_offset));
	update_flags(vm, dr);
}

/*
** Load Base + Offset:
** An address is computed by sign-extending bits [5:0] to 16 bits and adding this
** value to the contents of the register specified by bits [8:6]. The contents of
** memory at this address are loaded into DR. The condition codes are set, based
** on whether the value loaded is negative, zero, or positive.
*/
void op_ldr(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t base_r = extract_bits(instr, 6, 8);
	uint16_t offset = extend_bits(instr, 0, 5);
	vm->reg[dr] = mem_read(vm, vm->reg[base_r] + offset);
	update_flags(vm, dr);
}

/*
** Load Effective Address:
** An address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC. This address is loaded into DR.‡ The condition
** codes are set, based on whether the value loaded is negative, zero, or positive
*/
void op_lea(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);
	vm->reg[dr] = vm->reg[R_PC] + pc_offset;
	update_flags(vm, dr);
}

/*
** Not:
** The bit-wise complement of the contents of SR is stored in DR.
** The condition codes are set, based on whether the binary value produced,
** taken as a 2’s complement integer, is negative, zero, or positive.
*/
void op_not(struct vm_state *vm, uint16_t instr)
{
	uint16_t dr = extract_bits(instr, 9, 11);
	uint16_t sr = extract_bits(instr, 6, 8);

	vm->reg[dr] = ~vm->reg[sr];
	update_flags(vm, dr);
}

/*
** Return from Subroutine:
** The PC is loaded with the value in R7.
** This causes a return from a previous JSR instruction.
*/
void op_ret(struct vm_state *vm, uint16_t instr)
{
}

/*
** TODO
** Return from Interrupt:
** If the processor is running in Supervisor mode, the top two elements on the
** Supervisor Stack are popped and loaded into PC, PSR. If the processor is running
** in User mode, a privilege mode violation exception occurs.
*/
void op_rti(struct vm_state *vm, uint16_t instr)
{
	abort();
}

/*
** Store:
** The contents of the register specified by SR are stored in the memory location
** whose address is computed by sign-extending bits [8:0] to 16 bits and adding this
** value to the incremented PC.
*/
void op_st(struct vm_state *vm, uint16_t instr)
{
	uint16_t sr = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);
	mem_write(vm, vm->reg[R_PC] + pc_offset, vm->reg[sr]);
}

/*
** Store Indirect:
** The contents of the register specified by SR are stored in the memory location
** whose address is obtained as follows: Bits [8:0] are sign-extended to 16 bits and
** added to the incremented PC. What is in memory at this address is the address of
** the location to which the data in SR is stored.
 */
void op_sti(struct vm_state *vm, uint16_t instr)
{
	uint16_t sr = extract_bits(instr, 9, 11);
	uint16_t pc_offset = extend_bits(instr, 0, 8);
	mem_write(vm, mem_read(vm, vm->reg[R_PC] + pc_offset), vm->reg[sr]);
}

/*
** Store Base + Offset:
** The contents of the register specified by SR are stored in the memory location
** whose address is computed by sign-extending bits [5:0] to 16 bits and adding this
** value to the contents of the register specified by bits [8:6].
*/
void op_str(struct vm_state *vm, uint16_t instr)
{
	uint16_t sr = extract_bits(instr, 9, 11);
	uint16_t base_r = extract_bits(instr, 6, 8);
	uint16_t offset = extend_bits(instr, 0, 5);
	mem_write(vm, vm->reg[base_r] + offset, vm->reg[sr]);
}

/*
** TODO
*/
void op_res(struct vm_state *vm, uint16_t instr)
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
void op_trap(struct vm_state *vm, uint16_t instr)
{
	vm->reg[R_R7] = vm->reg[R_PC];
	trap(vm, instr & 0xff);
}

// lookup operation to apply to instruction
void apply_op(struct vm_state *vm, uint16_t op, uint16_t instr)
{
	switch (op) {
	case OP_ADD:
		op_add(vm, instr);
		break;
	case OP_AND:
		op_and(vm, instr);
		break;
	case OP_NOT:
		op_not(vm, instr);
		break;
	case OP_BR:
		op_br(vm, instr);
		break;
	case OP_JMP:
		op_jmp(vm, instr);
		break;
	case OP_JSR:
		op_jsr(vm, instr);
		break;
	case OP_LD:
		op_ld(vm, instr);
		break;
	case OP_LDI:
		op_ldi(vm, instr);
		break;
	case OP_LDR:
		op_ldr(vm, instr);
		break;
	case OP_LEA:
		op_lea(vm, instr);
		break;
	case OP_ST:
		op_st(vm, instr);
		break;
	case OP_STI:
		op_sti(vm, instr);
		break;
	case OP_STR:
		op_str(vm, instr);
		break;
	case OP_TRAP:
		op_trap(vm, instr);
		break;
	case OP_RES:
	case OP_RTI:
	default:
		abort();
		break;
	}
}
