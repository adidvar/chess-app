#include "turn.h"
#include <sstream>

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

std::string Turn::ToChessFormat() const
{
    if(from_.Valid() || to_.Valid())
    {
         return "NULL TURN";
    }
    else if(figure_ != Figure::kEmpty)
    {
        std::stringstream ss;
        ss << static_cast<char>('a'+from_.y()) << static_cast<char>('8'-from_.x()) << '-'
           << static_cast<char>('a'+to_.y())   << static_cast<char>('8'-to_.x());
        return ss.str();
    }
    else
    {
        char codes[7] = {' ','p','k','b','r','q',' '} ;

        std::stringstream ss;
        ss << static_cast<char>('a'+from_.y()) << static_cast<char>('8'-from_.x()) << '-'
           << static_cast<char>('a'+to_.y())   << static_cast<char>('8'-to_.x())
           << codes[(size_t)figure_];
        return ss.str();
     }
}

bool Turn::operator ==(const Turn& turn){
    return from_ == turn.from_ &&
           to_ == turn.to_ &&
           figure_ == turn.figure_;
}
