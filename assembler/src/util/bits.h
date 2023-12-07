#ifndef BITS_H_
#define BITS_H_

#include <stdint.h>

uint16_t swap16(uint16_t x);
uint16_t sign_extend(uint16_t x, int bit_count);
uint16_t extract_bits(uint16_t n, uint8_t a, uint8_t b);
uint16_t extend_bits(uint16_t n, uint8_t a, uint8_t b);
void write_bits(uint16_t *n, uint8_t a, uint8_t b, uint16_t value);

#endif // BITS_H_
