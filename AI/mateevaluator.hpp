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

class ValueAppraiser{
    int value_;

    static constexpr int min_ = std::numeric_limits<int>::min();
    static constexpr int max_ = std::numeric_limits<int>::max();
    static constexpr int mate_count_ = 10;
    static constexpr int min_mate_ = min_ + mate_count_;
    static constexpr int max_mate_ = max_ - mate_count_;

    ValueAppraiser(int value):
        value_(value){}
public:
    bool operator < (const ValueAppraiser &value) const {
        return value_ < value.value_;
    }
    bool operator > (const ValueAppraiser &value) const {
        return value_ > value.value_;
    }
    bool operator <= (const ValueAppraiser &value) const {
        return value_ <= value.value_;
    }
    bool operator >= (const ValueAppraiser &value) const {
        return value_ >= value.value_;
    }
    bool operator == (const ValueAppraiser &value) const {
        return value_ == value.value_;
    }
    bool operator != (const ValueAppraiser &value) const {
        return value_ != value.value_;
    }

    static ValueAppraiser CheckMateWin(int depth = 0){return ValueAppraiser{max_mate_+depth};};
    static ValueAppraiser CheckMateLose(int depth = 0){return ValueAppraiser{min_mate_-depth};};
    static ValueAppraiser Tie(){return ValueAppraiser{0};};
    static ValueAppraiser Approximate(BitBoard board, Color color)
    {

        int value = 0;
         value += 10*count_1s(board.GetBitBoard(Color::kWhite,Figure::kPawn));
         value -= 10*count_1s(board.GetBitBoard(Color::kBlack,Figure::kPawn));
         value += 30*count_1s(board.GetBitBoard(Color::kWhite,Figure::kKnight));
         value -= 30*count_1s(board.GetBitBoard(Color::kBlack,Figure::kKnight));
         value += 30*count_1s(board.GetBitBoard(Color::kWhite,Figure::kBishop));
         value -= 30*count_1s(board.GetBitBoard(Color::kBlack,Figure::kBishop));
         value += 50*count_1s(board.GetBitBoard(Color::kWhite,Figure::kRook));
         value -= 50*count_1s(board.GetBitBoard(Color::kBlack,Figure::kRook));
         value += 90*count_1s(board.GetBitBoard(Color::kWhite,Figure::kQueen));
         value -= 90*count_1s(board.GetBitBoard(Color::kBlack,Figure::kQueen));

         if(color == Color::kBlack)
             value = -value;

         return ValueAppraiser{value};
    };
    static ValueAppraiser Max(){return ValueAppraiser{max_};};
    static ValueAppraiser Min(){return ValueAppraiser{min_};};

    std::string ToString(size_t depth){
        if( value_ > max_mate_ && value_ < max_)
            return std::string("Win ") + std::to_string(depth-(value_-max_mate_));
        else if( value_ < min_mate_ && value_ > min_)
            return std::string("Lose ") + std::to_string(depth-(min_mate_-value_));
        else if (value_ == max_)
            return "Max";
        else if (value_ == min_)
            return "Min";
        else
            return std::to_string(value_);
    };
};

#endif
