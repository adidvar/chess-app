#pragma once
#include <AbstractBot.h>
#include <AbstractObs.h>

namespace Chess{

    class ChessController
    {

        public:
            ChessController(AbstractBot &wp , AbstractBot &bp):
            white_player(wp),
            black_player(bp),
            board()
            {}
            ChessController(AbstractBot &wp , AbstractBot &bp , Chessboard map):
            board(map),
            white_player(wp),
            black_player(bp)
            {}

            Chessboard GetGlobalBoard();

            bool NextMove();
            void SetObs(std::vector<AbstractObserver*> obs)
            {
                observers = obs;
            }

        private:
            Chessboard board;
            AbstractBot &white_player;
            AbstractBot &black_player;
            std::vector<AbstractObserver*> observers;

            Chessboard GetLocalBoard();
            static MatchStatus GetBoardStat(Chessboard &board);

    };
}

