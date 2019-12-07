#pragma once
#include "Chessboard.h"

namespace Chess
{

class ABot
{
protected:
    Chess::Color color;
public:
    ABot(Chess::Color c):
        color(c){}
    virtual bool GetTurn(Turn &turn) = 0;
};


};
