#include <AbstractChessController.h>

namespace Chess{

    class ChessController : public AbstractChessController
    {

        public:
            ChessController(AbstractBot &wp , AbstractBot &bp):
            white_player(wp),
            black_player(bp)
            {}
            ChessController(AbstractBot &wp , AbstractBot &bp , Chessboard map):
            AbstractChessController(map),
            white_player(wp),
            black_player(bp)
            {}

            bool NextMove();

        private:
            AbstractBot &white_player;
            AbstractBot &black_player;

    };
}

