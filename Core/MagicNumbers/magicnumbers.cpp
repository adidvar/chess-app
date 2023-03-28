#include "magicnumbers.h"
#include <cassert>
#include <cstdlib>
#include <exception>
#include <stdexcept>

Magic magic;

static uint64_t rmask1(uint64_t sq) {
  uint64_t result = 0;
  int rk = sq/8, fl = sq%8, r, f;
  for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
  for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
  return result;
}

static uint64_t rmask2(uint64_t sq) {
  uint64_t result = 0;
  int rk = sq/8, fl = sq%8, r, f;
  for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
  for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
  return result;
}

static uint64_t rattack1(uint64_t sq, uint64_t borders)
{
    uint64_t rmask = (rmask1(sq) << 1) | (rmask1(sq) >> 1);
    uint64_t p = (1<<sq);
    uint64_t result = 0;
    while(p & rmask){
        p <<= 1;
        result |= p;
        p &= ~borders;
    }
    p = (1<<sq);
    while(p & rmask){
        p >>= 1;
        result |= p;
        p &= ~borders;
    }
    return (result);
}

static uint64_t rattack2(uint64_t sq, uint64_t borders)
{
    uint64_t rmask = (rmask2(sq) << 8) | (rmask2(sq) >> 8);
    uint64_t p = (1<<sq);
    uint64_t result = 0;
    while(p & rmask){
        p <<= 8;
        result |= p;
        p &= ~borders;
    }
    p = (1<<sq);
    while(p & rmask){
        p >>= 8;
        result |= p;
        p &= ~borders;
    }
    return (result);
}




Magic::Magic()
{
    for(size_t p = 0 ; p < 64 ; p++){
        RMask1[p] = rmask1(p);
        RMask2[p] = rmask2(p);


    }
}
