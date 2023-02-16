#include "board.h"
#include <algorithm>
#include <stdexcept>

bool Board::ExecuteTurn(Turn turn)
{
    if(TurnTest(turn)==false)
        return false;

    Set(turn.to(),{GetFigure(turn.from()),GetColor(turn.from())});
    Set(turn.from(),{Figure::kEmpty,Color::kWhite});
    if(turn.figure() != Figure::kEmpty)
        Set(turn.to(),{turn.figure(),GetColor(turn.from())});

    /*
        bool direction = *value;
        auto swap_oo = [&board](unsigned h)
        {
        board.Swap(Position(h,4),Position(h,6));
        board.Swap(Position(h,7),Position(h,5));
        };
        auto swap_ooo = [&board](unsigned h)
        {
        board.Swap(Position(h,4),Position(h,2));
        board.Swap(Position(h,0),Position(h,3));
        };

        if(direction == false)
        swap_oo(board.current_color == white ? 7 : 0);
        else
        swap_ooo(board.current_color == white ? 7 : 0);

        board.r_flags = {false,false,false,false};
     */

    current_player_color_ = current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
    return true;
}

bool Board::TurnTest(Turn turn) const
{
      auto turns = GenerateTurns();
      auto count = std::count(turns.begin(),turns.end(),turn);

      return count != 0;
}
