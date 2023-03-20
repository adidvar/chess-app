#ifndef MAGICBOARDTOOLS
#define MAGICBOARDTOOLS
#include <inttypes.h>
#include <tuple>

uint64_t SlowMagic(uint64_t mask, uint64_t data);
uint64_t SlowMagicRev(uint64_t mask, uint64_t data);
std::pair<size_t,size_t> FindMagic(uint64_t mask);
std::pair<size_t,size_t> FindMagicRev(uint64_t mask);

#endif
