#pragma once
#include <Chessboard.h>

namespace Chess{

    class AbstractObserver
    {
    public:
        virtual void MapEvent(Chess::Chessboard map) = 0;


    };

}
