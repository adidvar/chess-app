#include <AbstractChessController.h>

namespace Chess{

    Chessboard AbstractChessController::GetGlobalBoard()
    {
        return board.whiteTurn ? board : ~board ;
    }

    Chessboard AbstractChessController::GetLocalBoard()
    {
        return board;
    }

    MatchStatus AbstractChessController::GetBoardStat(Chessboard &board)
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

}
