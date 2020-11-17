#ifndef PLAYER_H
#define PLAYER_H
#include "turn.h"
#include "board.h"

class Player
{
protected:
    Board board;
    Turn lastturn;
    Color main_color;
public:
    Player(Color main_color);
    ~Player(){};
    virtual void UploadMap(Board board , Turn lastturn);
    virtual Turn GetTurn() = 0;
};

#endif // PLAYER_H
