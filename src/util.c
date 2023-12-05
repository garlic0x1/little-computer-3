#include "util.h"
#include "io.h"

uint16_t swap16(uint16_t x)
{
	return (x << 8) | (x >> 8);
}

void update_flags(struct vm_state *state, uint16_t r)
{
	uint16_t r0 = state->reg[r];
	if (r0 == 0)
		state->reg[R_COND] = FL_ZRO;
	else if (r0 >> 15) /* a 1 in the left-most bit indicates negative */
		state->reg[R_COND] = FL_NEG;
	else
		state->reg[R_COND] = FL_POS;
}

void read_image_file(struct vm_state *state, FILE *file)
{
	uint16_t origin;
	fread(&origin, sizeof(origin), 1, file);
	// TODO
	origin = swap16(origin);

	uint16_t max_read = MEMORY_MAX - origin;
	uint16_t *p = state->memory + origin;
	size_t read = fread(p, sizeof(uint16_t), max_read, file);

	while (read-- > 0) {
		*p = swap16(*p);
		++p;
	}
}

int read_image(struct vm_state *state, const char *image_path)
{
	FILE *file = fopen(image_path, "rb");
	if (!file) return 0;
	read_image_file(state, file);
	fclose(file);
	return 1;
}

void mem_write(struct vm_state *state, uint16_t address, uint16_t val)
{
	state->memory[address] = val;
}

uint16_t mem_read(struct vm_state *state, uint16_t address)
{
	if (address == MR_KBSR) {
		if (check_key()) {
			state->memory[MR_KBSR] = (1 << 15);
			state->memory[MR_KBDR] = getchar();
		} else {
			state->memory[MR_KBSR] = 0;
		}
	}
	return state->memory[address];
}
