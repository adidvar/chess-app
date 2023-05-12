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

class MateAppraiser{
    int value_;
    MateAppraiser(int value):
        value_(value){}
public:
    bool operator < (const MateAppraiser &value) const {
        return value_ < value.value_;
    }

    static MateAppraiser CheckMateWin(){return MateAppraiser{20};};
    static MateAppraiser CheckMateLose(){return MateAppraiser{-20};};
    static MateAppraiser Tie(){return MateAppraiser{-1};};
    static MateAppraiser Approximate(BitBoard board, Color color){return MateAppraiser{0};};
    static MateAppraiser Max(){return MateAppraiser{100};};
    static MateAppraiser Min(){return MateAppraiser{-100};};

    std::string ToString(){
        switch(value_){
            case -1:
                return "Tie";
            case 0:
                return "Not enought depth";
        }
        if( value_ > 10)
            return std::string("Win ") + std::to_string(20-value_);
        if( value_ < -10)
            return std::string("Lose ") + std::to_string(20+value_);
    };

    MateAppraiser Process() const {
        if(value_ <= 20 && value_ > 10)
            return MateAppraiser(value_-1);
        else if(value_ < -10 && value_ >= -20)
            return MateAppraiser(value_+1);
        else
            return MateAppraiser(value_);
    };
};

#endif
