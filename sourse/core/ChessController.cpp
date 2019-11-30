#include "ChessController.h"

namespace Chess{

;

Chessboard ChessController::GetGlobalBoard()
{
    return board.WhoTurn() ? board : ~board ;
}

Chessboard ChessController::GetLocalBoard()
{
    return board;
}

bool ChessController::NextMove()
{
    Turn t;
    ( board.WhoTurn() ? white_player : black_player)->MapEvent(board);
    do{
    ( board.WhoTurn() ? white_player : black_player)->GetTurn(t);
    }while(board.makeTurn(t,true)==false);

    for(AObserver *o : observers)
    {
        o->MapEvent(this->GetGlobalBoard());
    }

    auto stat = Chessboard::GetBoardStat(board);

    if(stat!=Now)
    {
        if(stat == Win)
        {
           ( board.WhoTurn() ? white_player : black_player)->YouWin();
           ( !board.WhoTurn() ? white_player : black_player)->YouLose();
        }
        if(stat == Lose)
        {
           ( board.WhoTurn() ? white_player : black_player)->YouLose();
           ( !board.WhoTurn() ? white_player : black_player)->YouWin();
        }
        return false;
    }

    board = ~board;
    return true;
}
	
};

