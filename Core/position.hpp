#ifndef POSITION_H
#define POSITION_H
#include <cinttypes>
#include <string>
#include <cassert>

/*
+--+--+--+--+--+--+--+--+
| 0| 1| 2| 3| 4| 5| 6| 7|
+--+--+--+--+--+--+--+--+
| 8| 9|10|11|12|13|14|15|
+--+--+--+--+--+--+--+--+
|16|17|18|19|20|21|22|23|
+--+--+--+--+--+--+--+--+
|24|25|26|27|28|29|30|31|
+--+--+--+--+--+--+--+--+
|32|33|34|35|36|37|38|39|
+--+--+--+--+--+--+--+--+
|40|41|42|43|44|45|46|47|
+--+--+--+--+--+--+--+--+
|48|49|50|51|52|53|54|55|
+--+--+--+--+--+--+--+--+
|56|57|58|59|60|61|62|63|
+--+--+--+--+--+--+--+--+
*/

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
            index_ = y*8 + x;
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
        return index_%8;
    }

    constexpr uint8_t y() const noexcept{
        return index_/8;
    }

    constexpr Position Rotate() const noexcept{
        if(Valid())
            return Position(63-index_);
        else
            return Position();
    }

    std::string ToString() const noexcept
    {
        std::string str = "00";
        str[0] = static_cast<char>('a'+x());
        str[1] = static_cast<char>('8'-y());
        if(Valid())
            return str;
        else
            return "--";
    }

    static Position FromString(std::string_view string) noexcept
    {
        if(string.size()!=2)
            return Position();
        else if(string[0] >= 'a' && string[0] <= 'h' && string[1] >= '1' && string[1] <= '8')
            return Position(string[0]-'a', '8'-string[1]);
        else
            return Position();
    }

    bool operator ==(const Position& pos) const noexcept{
        return index_ == pos.index_;
    }

    bool operator !=(const Position& pos) const noexcept{
        return index_ != pos.index_;
    }

    bool operator <(const Position& pos) const noexcept{
        return index_ < pos.index_;
    }

    constexpr static uint8_t Max() { return kerror_pos_;};
};

#endif // POSITIONS_H
