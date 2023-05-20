#ifndef POSITION_RATING_HPP
#define POSITION_RATING_HPP
#include <string>
#include <bitboard.hpp>

class Appraiser{
    int value_;
public:
    bool operator < (const Appraiser &value) const {
        return value_ < value.value_;
    }

    static Appraiser CheckMateWin();
    static Appraiser CheckMateLose();
    static Appraiser Tie();
    static Appraiser Approximate(BitBoard board);

    Appraiser Process() const;
};


#endif
