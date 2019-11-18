#pragma once
#include <Chessboard.h>

namespace Chess
{
	
	class AbstractBot
	{
		public:
		virtual bool GetTurn(Turn &t,Chessboard &board) = 0;
        virtual void YouLose() = 0;
        virtual void YouWin() = 0;
	};
	
	
};
