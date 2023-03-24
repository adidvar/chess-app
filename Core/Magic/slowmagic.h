#ifndef SLOWMAGIC_H
#define SLOWMAGIC_H
#include <inttypes.h>

size_t CountOnes(uint64_t number);
uint64_t Encode(uint64_t mask, uint64_t data);
uint64_t Decode(uint64_t mask, uint64_t data);

#endif
