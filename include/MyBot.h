#pragma once
#include "AbstractBot.h"
#include <array>
#include <iostream>

namespace Chess{

    enum AI_Mode
    {
      Easy = 1,
      //Medium = 2,
      Hard = 3
    };

class MyBot : public Chess::AbstractBot
{
    AI_Mode mode;
	public:
    MyBot(AI_Mode md):
        mode(md)
    {}
    MyBot():
        mode(Hard)
    {}
		bool GetTurn(Turn &t,Chessboard &board);
        float GetTreeMark(Chessboard &board , bool MinMax , unsigned TTL );
        float GetPositionMark(Chessboard &board);
        void YouWin(){}
        void YouLose(){}
    };
};
