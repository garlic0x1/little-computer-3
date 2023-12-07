#include "bits.h"
#include "util.h"
#include <stdint.h>

uint16_t swap16(uint16_t x)
{
	return (x << 8) | (x >> 8);
}

uint16_t sign_extend(uint16_t x, int bit_count)
{
	if ((x >> (bit_count - 1)) & 1) {
		x |= (0xFFFF << bit_count);
	}
	return x;
}

uint16_t extract_bits(uint16_t n, uint8_t a, uint8_t b)
{
	// removing check will make faster...
	if (a > 15 || b > 15 || a > b)
		return 0;
	else
		return (n >> a) & ((1 << (b - a + 1)) - 1);
}

uint16_t extend_bits(uint16_t n, uint8_t a, uint8_t b)
{
	return sign_extend(extract_bits(n, a, b), (b - a) + 1);
}

void write_bits(uint16_t *n, uint8_t a, uint8_t b, uint16_t value)
{
	if (a > 15 || b > 15 || a > b)
		return;
	uint16_t mask = ((1 << (b - a + 1)) - 1) << a;
	*n &= ~mask;
	*n |= (value << a) & mask;
}
