#include "turn.h"
#include <sstream>

Turn::Turn(std::tuple<Pos,Pos> move):
    turn(move)
{}

Turn::Turn(std::tuple<Pos,Pos,Figures> pawn_move):
    turn(pawn_move)
{}

Turn::Turn(Rooking direction):
    turn(direction)
{}

std::optional<std::tuple<Pos, Pos> > Turn::GetTurn() const
{
    if(auto value = std::get_if<std::tuple<Pos, Pos>>(&turn))
        return *value;
    return {};
}

std::optional<std::tuple<Pos, Pos, Figures> > Turn::GetPawnTurn() const
{
    if(auto value = std::get_if<std::tuple<Pos, Pos,Figures>>(&turn))
        return *value;
    return {};
}

std::optional<Rooking> Turn::GetRooking() const
{
    if(auto value = std::get_if<bool>(&turn))
        return *value;
    return {};
}

std::string Turn::toChessFormat() const
{
    if(auto value = std::get_if<std::tuple<Pos, Pos>>(&turn))
    {
        auto& [start , end] = *value;
        std::stringstream ss;
        ss << static_cast<char>('a'+Position_y(start)) << static_cast<char>('8'-Position_x(start)) << '-'
           << static_cast<char>('a'+Position_y(end))   << static_cast<char>('8'-Position_x(end));
        return ss.str();
    }
    else if (auto value = std::get_if<std::tuple<Pos, Pos , Figures>>(&turn))
    {
        char codes[7] = {' ',' ','k','b','r','q',' '} ;

        auto& [start , end , figure] = *value;
        std::stringstream ss;
        ss << static_cast<char>('a'+Position_y(start)) << static_cast<char>('8'-Position_x(start)) << '-'
           << static_cast<char>('a'+Position_y(end))   << static_cast<char>('8'-Position_x(end))
           << codes[figure];
        return ss.str();

     }
     else if (const bool *value = std::get_if<Rooking>(&turn))
     {
            bool direction = *value;
            if(direction == false)
                return "0-0";
            else
                return "0-0-0";
     }
     else
     {
         return "NULL TURN";
      }
}

bool Turn::operator ==(const Turn &t) const
{
    return turn == t.turn;
}
