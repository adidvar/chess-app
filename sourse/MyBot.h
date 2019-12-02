#pragma once
#include "APlayer.h"
#include <array>
#include <iostream>

namespace Chess{

enum AI_Mode
{
    Easy = 1,
    //Medium = 2,
    Hard = 3
};

class MyBot : public Chess::APlayer
{
    AI_Mode mode;
    Chessboard board;

    float GetTreeMark(Chessboard &board , bool MinMax , unsigned TTL );
    float GetPositionMark(Chessboard &board);

public:

    MyBot(AI_Mode md , Color c):
        APlayer(c),
        mode(md)
    {}
    MyBot(Color c):
        APlayer(c),
        mode(Hard)
    {}

    void MapEvent(Chessboard board)
    {
        this->board = board;
    }
    bool GetTurn(Turn &t);
    void YouWin(){}
    void YouLose(){}
};
};
