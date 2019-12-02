#pragma once
#include "Chessboard.h"

namespace Chess{

class AObserver
{
public:
    virtual void MapEvent(Chess::Chessboard map) = 0;
};

}
