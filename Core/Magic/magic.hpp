#ifndef MAGIC_HPP
#define MAGIC_HPP
#include <cinttypes>

void InitMagic();

uint64_t ProcessRook(uint64_t sq, uint64_t borders);
uint64_t ProcessBishop(uint64_t sq, uint64_t borders);

#endif
