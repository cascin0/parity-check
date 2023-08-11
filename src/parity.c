#include "parity.h"

int has_even_parity(uint8_t x)
{
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;

  return (~x) & 1;
}
