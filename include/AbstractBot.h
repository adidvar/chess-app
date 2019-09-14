#include "ChessBoard.h"

namespace Chess
{
	
	class AbstractBot
	{
		public:
		virtual bool GetTurn(Turn &t,Chessboard &board) = 0;
	};
	
	
};