#include "ChessController.h"
#include <array>
#include <iostream>

namespace Chess{

class MyBot : public Chess::AbstractBot
{
	public:
		bool GetTurn(Turn &t,Chessboard &board);
        void GetTreeMark(float &TreeMark , bool &TreeResult , Chessboard &board , bool MinMax , unsigned TTL );
        void GetPositionMark(float &PositionMark , Chessboard &board);
};


};
