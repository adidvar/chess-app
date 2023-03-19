#ifndef MAGICBOARDTOOLS
#define MAGICBOARDTOOLS
#include <inttypes.h>
#include <tuple>

uint64_t SlowMagic(uint64_t mask, uint64_t data);
std::pair<size_t,size_t> FindMagic(uint64_t mask);

#endif
