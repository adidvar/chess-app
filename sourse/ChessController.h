#pragma once
#include "APlayer.h"

namespace Chess{

    enum ControllerStatus
    {
        Good,
        Error
    };

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
    MatchStatus GetMatchResult();
    std::vector<Chessboard> GetMatchHistory();

    bool NextMove();

    void SetObs(std::vector<AObserver*> obs);

private:
    Chessboard board;
    APlayer *white_player;
    APlayer *black_player;
    std::vector<AObserver*> observers;
    std::vector<Chessboard> history;
    Chessboard GetLocalBoard();
    ControllerStatus status = Good;
};
}

