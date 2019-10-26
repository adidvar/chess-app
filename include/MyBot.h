#include "ChessController.h"
#include <array>
#include <iostream>

namespace Chess{

class MyBot : public Chess::AbstractBot
{
	public:
		bool GetTurn(Turn &t,Chessboard &board);
        float GetTreeMark(Chessboard &board , bool MinMax , unsigned TTL );
        float GetPositionMark(Chessboard &board);
        void YouWin(){}
        void YouLose(){}
    };
};
