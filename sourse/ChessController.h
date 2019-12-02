#pragma once
#include "APlayer.h"

namespace Chess{

class ChessController
{

public:
    ChessController(APlayer *wp , APlayer *bp):
        white_player(wp),
        black_player(bp),
        board()
    {}
    ChessController(APlayer *wp , APlayer *bp , Chessboard map):
        board(map),
        white_player(wp),
        black_player(bp)
    {}

    Chessboard GetGlobalBoard();

    bool NextMove();
    void SetObs(std::vector<AObserver*> obs)
    {
        observers = obs;
    }

private:
    Chessboard board;
    APlayer *white_player;
    APlayer *black_player;
    std::vector<AObserver*> observers;

    Chessboard GetLocalBoard();
    static MatchStatus GetBoardStat(Chessboard &board);

};
}

