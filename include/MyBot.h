#include "ChessController.h"
#include <array>

namespace Chess{

class MyBot : public Chess::AbstractBot
{
	public:
		bool GetTurn(Turn &t,Chessboard &board);
        void GetTreeMark(double &TreeMark , bool &TreeResult , Chessboard &board , bool MinMax , unsigned TTL );
        void GetPositionMark(double &PositionMark , Chessboard &board);
};


};
