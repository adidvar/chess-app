#include "ChessController.h"

namespace Chess{

    bool ChessController::NextMove()
    {
        Turn t;
        do{
        ( board.whiteTurn ? white_player : black_player).GetTurn(t,board);
        }while(board.makeTurn(t,true)==false);

        auto stat = GetBoardStat(board);

        if(stat!=Now)
        {
            if(stat == Win)
            {
               ( board.whiteTurn ? white_player : black_player).YouWin();
               ( !board.whiteTurn ? white_player : black_player).YouLose();
            }
            if(stat == Lose)
            {
               ( board.whiteTurn ? white_player : black_player).YouLose();
               ( !board.whiteTurn ? white_player : black_player).YouWin();
            }
            return false;
        }

        board = ~board;
        board.whiteTurn = !board.whiteTurn;
        return true;
    }
	
};

