#include "turn.hpp"
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

bool Turn::Valid() const noexcept
{
    return from_.Valid() && to_.Valid();
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

bool Turn::operator ==(const Turn& turn){
    return from_ == turn.from_ &&
           to_ == turn.to_ &&
           figure_ == turn.figure_;
}
