#ifndef MATEEVALUATOR_HPP
#define MATEEVALUATOR_HPP

#include <string>
#include <bitboard.hpp>

#include <limits>
#include <magic.hpp>

class MateAppraiser{
    int value_;


    MateAppraiser(int value):
        value_(value){}

public:
    bool operator < (const MateAppraiser &value) const {
        return value_ < value.value_;
    }
    bool operator > (const MateAppraiser &value) const {
        return value_ > value.value_;
    }
    bool operator <= (const MateAppraiser &value) const {
        return value_ <= value.value_;
    }
    bool operator >= (const MateAppraiser &value) const {
        return value_ >= value.value_;
    }
    bool operator == (const MateAppraiser &value) const {
        return value_ == value.value_;
    }
    bool operator != (const MateAppraiser &value) const {
        return value_ != value.value_;
    }

    static MateAppraiser CheckMateWin(){return MateAppraiser{20};};
    static MateAppraiser CheckMateLose(){return MateAppraiser{-20};};
    static MateAppraiser Tie(){return MateAppraiser{0};};
    static MateAppraiser Approximate(BitBoard board, Color color){return MateAppraiser{0};};
    static MateAppraiser Max(){return 100;};
    static MateAppraiser Min(){return -100;};

    std::string ToString(){
        switch(value_){
            case -1:
                return "Tie";
            case 0:
                return "Not enought depth";
        }
        if( value_ > 10 && value_ <= 20)
            return std::string("Win ") + std::to_string(20-value_);
        else if( value_ < -10 && value_ >= -20)
            return std::string("Lose ") + std::to_string(20+value_);
        else if (value_ == 100)
            return "Max";
        else if (value_ == -100)
            return "Min";
        else
            return "Error";
    };
};

#endif
