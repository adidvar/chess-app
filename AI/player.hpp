#ifndef PLAYER_H
#define PLAYER_H

#include <turn.hpp>
#include <bitboard.hpp>

class Player
{
public:
    Player(Color main_color);
    ~Player(){};
    virtual Turn GetTurn(BitBoard board) = 0;
    virtual float Evaluate(BitBoard board) = 0;
};

#endif // PLAYER_H
