#pragma once
#include <Chessboard.h>

namespace Chess
{

class ABot
{
    Chess::Color color;
public:
    ABot(Chess::Color c):
        color(c){}
    virtual bool GetTurn(Turn &turn) = 0;
    virtual void YouLose() = 0;
    virtual void YouWin() = 0;
};


};
