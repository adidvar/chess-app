#include "turn.hpp"
#include <sstream>

const static Turn white_long_castling(60,58), black_long_castling(4,2),
            white_short_castling(60,62), black_short_castling(4,6);

Turn::Turn():
    figure_(Figure::kEmpty)
{

}

Turn::Turn(Position from, Position to):
    from_(from),
    to_(to),
    figure_(Figure::kEmpty)
{

}

Turn::Turn(Position from, Position to, Figure figure):
    from_(from),
    to_(to),
    figure_(figure)
{

}

Position Turn::from() const noexcept
{
   return from_;
}

Position Turn::to() const noexcept
{
   return to_ ;
}

Figure Turn::figure() const noexcept
{
    return figure_ ;
}

bool Turn::Valid() const noexcept
{
    return from_.Valid() && to_.Valid();
}

bool Turn::IsCastling() const noexcept
{
    return IsLongCastling() || IsShortCastling();
}

bool Turn::IsLongCastling() const noexcept
{
    return (*this) == white_long_castling || (*this) == black_long_castling;
}

bool Turn::IsShortCastling() const noexcept
{
    return (*this) == white_short_castling || (*this) == black_short_castling;
}

bool Turn::IsTrasformation() const noexcept
{
    return figure_ != Figure::kEmpty;
}

std::string Turn::ToChessFormat() const
{
    if(!Valid())
    {
         return "NULL TURN";
    }
    else if(figure_ == Figure::kEmpty)
    {
        std::stringstream ss;
        ss << from_.ToString() << to_.ToString();
        return ss.str();
    }
    else
    {
        char codes[7] = {' ','p','k','b','r','q',' '} ;

        std::stringstream ss;
        ss << from_.ToString() << to_.ToString()
           << codes[(size_t)figure_];
        return ss.str();
     }
}

bool Turn::operator ==(const Turn& turn) const{
    return from_ == turn.from_ &&
           to_ == turn.to_ &&
           figure_ == turn.figure_;
}
