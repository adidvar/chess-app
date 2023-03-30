#pragma once
#ifndef APLAYER_H
#define APLAYER_H

#endif // APLAYER_H
#include "AObs.h"
#include "ABot.h"

namespace Chess
{

class APlayer : public Chess::AObserver, public Chess::ABot
{
public:
    APlayer(Color c):ABot(c){};
    virtual ~APlayer(){};
};
};
