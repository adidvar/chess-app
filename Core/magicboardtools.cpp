#include "magicboardtools.h"
#include <cassert>

size_t CountOnes(uint64_t number){
    size_t counter = 0;
    while(number!=0){
        number = (number-1)&number;
        counter++;
    }
    return counter;
}

uint64_t SlowMagic(uint64_t mask, uint64_t data){
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

#include <iostream>
void PrintBoard(uint64_t board){
    using namespace std;
    for(size_t x = 0 ; x < 64 ;x++){
        if(x%8==0){
            cout << endl;
        }
        cout << ((board>>x)&1);
    }
    cout << endl;
}

std::pair<size_t,size_t> FindMagic(uint64_t mask)
{
    uint64_t tests[2 << 10];
    size_t tests_size = 1<<CountOnes(mask);
    assert(tests_size < (2 << 10));

    for(size_t i = 0; i < tests_size ; i++ ){
        tests[i] = SlowMagic(mask,i);
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
        if(flag)
            return {magic,0};
    }
}
