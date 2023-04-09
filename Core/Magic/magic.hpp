#ifndef MAGIC_HPP
#define MAGIC_HPP
#include <cinttypes>

uint64_t log2_64(uint64_t value);
int count_1s(uint64_t b);

void InitMagic();

uint64_t ProcessRook(uint64_t sq, uint64_t borders);
uint64_t ProcessBishop(uint64_t sq, uint64_t borders);

uint64_t AttackFrom(uint64_t sq);

#endif
