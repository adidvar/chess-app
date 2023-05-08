#ifndef POSITION_RATING_HPP
#define POSITION_RATING_HPP
#include <string>

class PositionRating{
    int value_;

    PositionRating(int value):
    value_(value)
    {}
public:
    static constexpr int max_value = 1000;
    static PositionRating FromValue(int value){
        return PositionRating{value};
    }
    /*
    static PositionRating FromValue(int value){
        return PositionRating{value};
    }
    */
    int Value()
    {
        return value_;
    }
    std::string ToString()
    {
        return std::to_string(value_);
    }
};

#endif
