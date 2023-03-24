#include "magic.h"
#include "slowmagic.h"
#include <cassert>
#include <cstdlib>
#include <exception>
#include <stdexcept>

bool TestNum();

struct MagicPair
{
    uint64_t magic;
    uint64_t shift;
};

static MagicPair FastMagic(uint64_t mask)
{
    MagicPair pair;
    uint64_t tests[2 << 10];
    size_t tests_size = 1<<CountOnes(mask);
    assert(tests_size < (2 << 10));

    for(size_t i = 0; i < tests_size ; i++ ){
        tests[i] = Encode(mask,i);
    }

    for(uint64_t i = 0 ; ;++i)
    {
        uint64_t magic = 0;
        for(size_t j = 0 ; j < CountOnes(mask); j++)
        {

        }
        bool flag = true;
        for(size_t i = 0 ; i < tests_size ; i++)
            if((((i*magic))&mask) != tests[i])
            {
                flag = false;
                break;
            }
        if(flag){
            pair.magic = magic;
            break;
        }
    }
}

static MagicPair BruteForceMagic(uint64_t mask)
{
    MagicPair pair;
    uint64_t tests[2 << 10];
    size_t tests_size = 1<<CountOnes(mask);
    assert(tests_size < (2 << 10));

    for(size_t i = 0; i < tests_size ; i++ ){
        tests[i] = Encode(mask,i);
    }

    for(size_t magic = 0 ; ; magic++)
    {
        bool flag = true;
        for(size_t i = 0 ; i < tests_size ; i++)
            if((((i*magic))&mask) != tests[i])
            {
                flag = false;
                break;
            }
        if(flag){
            pair.magic = magic;
            break;
        }
    }
    for(uint64_t shift = 0 ; shift < 64 ; ++shift)
    {
        bool flag = true;
        for(size_t i = 0 ; i < tests_size ; i++)
            if((((tests[i]*pair.magic) >> shift)&((1LL<<CountOnes(mask))-1)) != i)
            {
                flag = false;
                break;
            }
        if(flag){
            pair.shift = shift;
            break;
        }
    }
    return pair;
}

Magic::Magic(uint64_t mask):
    mask_(mask)
{
    plot_mask_ = (1LL<<CountOnes(mask))-1;
    auto pair = BruteForceMagic(mask);
    magic_ = pair.magic;
    shift_ = pair.shift;

    if (((MagicNum()*Mask() >> Shift()) & PlotMask()) != PlotMask())
    {
       throw std::runtime_error("Magic error");
    }
}

Magic::Magic(uint64_t mask, uint64_t magic, uint64_t shift):
    mask_(mask),
    magic_(mask),
    plot_mask_((1LL<<CountOnes(mask))-1),
    shift_(shift)
{
    if (((MagicNum()*Mask() >> Shift()) & PlotMask()) != PlotMask())
    {
       throw std::runtime_error("Magic error");
    }
}

uint64_t Magic::Encode(uint64_t value) const
{
    return value*magic_ & mask_;
}

uint64_t Magic::Decode(uint64_t value) const
{
    return ((value*magic_) >> shift_) & plot_mask_;
}
