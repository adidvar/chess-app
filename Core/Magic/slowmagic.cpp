#include "slowmagic.h"
#include <cassert>

size_t CountOnes(uint64_t number)
{
    size_t counter = 0;
    while(number!=0){
        number = (number-1)&number;
        counter++;
    }
    return counter;
}
/// from plot to mask
uint64_t Encode(uint64_t mask, uint64_t data)
{
    uint64_t counter = 0;
    uint64_t result = 0;
    while(mask!=0)
    {
        uint64_t bit = mask^((mask-1)&mask);
        uint64_t value = (data >> counter) &1uLL;

        if(value)
            value = 0xFFFFFFFFFFFF;

        result |= value & bit;

        mask &= mask-1;
        counter++;
    }
    return result;
}

///from mask to plot
uint64_t Decode(uint64_t mask, uint64_t data)
{
    uint64_t counter = 0;
    uint64_t result = 0;
    while(mask!=0)
    {
        uint64_t bit = mask^((mask-1)&mask);
        uint64_t value = data & bit;

        if(value)
            value = 0xFFFFFFFFFFFF;

        result |= value & (1LL << counter);

        mask &= mask-1;
        counter++;
    }
    return result;
}
