#include "ChessController.h"

namespace Chess{

;

Chessboard ChessController::GetGlobalBoard()
{
    return board.whiteTurn ? board : ~board ;
}

Chessboard ChessController::GetLocalBoard()
{
    return board;
}

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

MatchStatus ChessController::GetBoardStat(Chessboard &board)
{
    int MyKingCount = 0;
    int HisKingCount = 0;
    for(auto p : board.map)
    {
        if(p.type == King && p.color == White)
            MyKingCount++;
        if(p.type == King && p.color == Black)
            HisKingCount++;
    }

    if(MyKingCount > 0 && HisKingCount > 0)
        return MatchStatus::Now;
    if(MyKingCount <= 0 && HisKingCount > 0)
        return MatchStatus::Lose;
    if(MyKingCount > 0 && HisKingCount <= 0)
        return MatchStatus::Win;
}

	
};

