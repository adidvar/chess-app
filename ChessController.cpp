#include "ChessController.h"
#include <iostream>


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
    bool stat =  ( board.whiteTurn ? white_player : black_player).GetTurn(t,board);
	
    if(!board.makeTurn(t,true))
		std::cerr << "Uncorect MOVE! Player:" << (board.whiteTurn ? "White" : "Black") << std::endl;
    board = ~board;
	board.whiteTurn = !board.whiteTurn;
    return stat;
}

	
};

