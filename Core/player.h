#ifndef PLAYER_H
#define PLAYER_H
#include "turn.h"
#include "board.h"

class Player
{
protected:
    Board board;
    Position lastturn;
    Color main_color;
public:
    Player(Color main_color);
    ~Player(){};
    virtual void UploadMap(Board board , Position lastturn);
    virtual Position GetTurn() = 0;
};

#endif // PLAYER_H
