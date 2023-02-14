#include "turnexecutor.h"
#include "turngenerator.h"
#include "algorithm"
#include <stdexcept>

bool ExecuteTurn(Board &board, Turn turn)
{
    if(TurnTest(board,turn)==false)
        return false;

    auto variant = turn.variant();
    if(auto value = std::get_if<std::tuple<Pos, Pos>>(&variant))
    {
        auto& [start , end] = *value;
        board.Set(end,board.GetFigure(start),board.GetColor(start));
        board.Set(start,empty,white);

    }
    else if (auto value = std::get_if<std::tuple<Pos, Pos , Figures>>(&variant))
    {
        auto& [start , end , figure] = *value;
        board.Set(end,figure,board.GetColor(start));
        board.Set(start,empty,white);
     }
     else if (bool *value = std::get_if<bool>(&turn.turn))
     {
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
     }
     else
     {
         throw std::runtime_error("Making a null turn");
     }

     board.current_color = board.current_color == white ? black : white;
     return true;
}

bool TurnTest(const Board &board, const Turn turn)
{
      auto turns = TurnGenerate(board,board.CurrentColor());
      auto count = std::count(turns.begin(),turns.end(),turn);

      return count != 0;
}
