#ifndef ABSTRACTBOT_H
#define ABSTRACTBOT_H

#endif // ABSTRACTBOT_H

#include <AbstractBot.h>
#include <AbstractObserver.h>

namespace Chess{

    class AbstractChessController
    {

        public:
        AbstractChessController();
        AbstractChessController(Chessboard map):
            board(map){}

            Chessboard GetGlobalBoard();
            Chessboard GetLocalBoard();

            virtual bool NextMove() = 0;
            void SetObservers(std::vector<AbstractObserver*> obs);

        protected:
            virtual MatchStatus GetBoardStat(Chessboard &board);
            Chessboard board;

    };
}
