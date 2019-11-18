#ifndef ABSTRACTOBS_H
#define ABSTRACTOBS_H

#endif // ABSTRACTOBS_H

#include <Chessboard.h>

class AbstractObserver
{
public:
    virtual void MapEvent(Chess::Chessboard map) = 0;


};
