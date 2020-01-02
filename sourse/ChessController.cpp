#include "ChessController.h"

namespace Chess{

;

Chessboard ChessController::GetGlobalBoard()
{
    return board.WhoTurn() ? board : ~board ;
}

MatchStatus ChessController::GetMatchResult()
{
    Chessboard brd = this->GetGlobalBoard();
    return Chessboard::GetBoardStat(brd);
}

std::vector<Chessboard> ChessController::GetMatchHistory()
{
    return this->history;
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
    history.push_back(this->GetGlobalBoard());

    auto stat = Chessboard::GetBoardStat(board);

    if(stat!=Now)
    {
        if(stat == Win)
        {
            ( !board.WhoTurn() ? white_player : black_player)->MapEvent(board);
           ( board.WhoTurn() ? white_player : black_player)->FinishEvent(Color::White);
           ( !board.WhoTurn() ? white_player : black_player)->FinishEvent(Color::Black);
        }
        if(stat == Lose)
        {
           ( !board.WhoTurn() ? white_player : black_player)->MapEvent(board);
           ( board.WhoTurn() ? white_player : black_player)->FinishEvent(Color::Black);
           ( !board.WhoTurn() ? white_player : black_player)->FinishEvent(Color::White);
        }

        for(AObserver *o : observers)
        {
            Chessboard gb = this->GetGlobalBoard();
            o->FinishEvent( Chessboard::GetBoardStat(gb) == Win ? White : Black );
        }

        return false;
    }

    board = ~board;
    return true;
}

void ChessController::SetObs(std::vector<AObserver *> obs)
{
    observers = obs;
}
	
};

