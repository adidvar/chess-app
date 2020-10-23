#pragma once
#include <iostream>
#include "player.h"
#include <thread>
#include <mutex>
#include <limits>
#include "turngenerator.h"
#include "turnexecutor.h"

class Bot : public Player
{

    const unsigned depth = 5;

    virtual int16_t MarkTreeBranch(Board board , unsigned depth , int16_t alpha , int16_t beta );
    virtual int16_t Mark(Board &board);

public:
    Bot(Color c):
        Player(c)
    {}
    virtual Turn GetTurn();
};
