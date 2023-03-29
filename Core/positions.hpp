#ifndef POSITIONS_H
#define POSITIONS_H
#include <cinttypes>
#include <string>
#include <cassert>

class Position
{
    static constexpr uint8_t kerror_pos_ = 64;
    uint8_t index_;
public:
    Position(uint8_t x , uint8_t y) noexcept
    {
        if( x > 7 || y > 7)
            index_ = kerror_pos_;
        else
            index_ = x*8 + y;
    }
    constexpr Position() noexcept:
        index_(kerror_pos_)
    {
    }
    constexpr Position(uint8_t index) noexcept:
        index_(index)
    {
    }
    constexpr bool Valid() const noexcept
    {
        return index_ < kerror_pos_;
    }
    constexpr uint8_t Value() const noexcept{
        return index_;
    }
    constexpr uint8_t x() const noexcept{
        return index_/8;
    }
    constexpr uint8_t y() const noexcept{
        return index_%8;
    }

    std::string ToString() const noexcept
    {
        std::string str = "00";
        str[0] = static_cast<char>('a'+y());
        str[1] = static_cast<char>('8'-x());
        return str;
    }
    bool operator ==(const Position& pos) const noexcept{
        return index_ == pos.index_;
    }
};

#endif // POSITIONS_H
