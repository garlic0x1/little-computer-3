#include "bits.h"
#include <stdint.h>

uint16_t rev_endian(uint16_t n)
{
    uint16_t res = 0;
    while (n > 0) {
        res += n % 2;

        n /= 2;
    }
    return res;
}

uint16_t extract_bits(uint16_t n, uint8_t a, uint8_t b)
{
	if (a > 15 || b > 15 || a > b)
		return 0;
	else
        return n & ((1 << (b - a + 1)) - 1) << a;
		/* return (n >> a) & ((1 << (b - a + 1)) - 1); */
}

uint16_t _extract_bits(uint16_t n, uint8_t a, uint8_t b)
{
    return n & ((1 << (b - a + 1)) - 1) << a;
}

void write_bits(uint16_t *n, uint8_t a, uint8_t b, uint16_t value)
{
	if (a > 15 || b > 15 || a > b)
		return;
	uint16_t mask = ((1 << (b - a + 1)) - 1) << a;
	*n &= ~mask;
	*n |= (value << a) & mask;
}
