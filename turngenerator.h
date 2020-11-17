#ifndef TURNGENERATOR_H
#define TURNGENERATOR_H
#include "board.h"
#include "turn.h"
#include <queue>

class TurnWrapper
{
public:
    TurnWrapper(Turn turn , int8_t priority):
        turn(turn),
        priority(priority)
    {}
    Turn turn;
    int8_t priority;
};

class TurnGenerator
{
    const Board &board;
    std::priority_queue<TurnWrapper> turns;

    void position_execute(const int8_t offset_x[] ,const int8_t offset_y[] , size_t size , uint8_t x , uint8_t y , int8_t priority_move , int8_t priority_atack);
    void positon_execute_resursive(const int8_t offset_x[] , const int8_t offset_y[] , size_t size , uint8_t x , uint8_t y , int8_t priority_move , int8_t priority_atack);

public:
    TurnGenerator(const Board &board);

    operator bool() const; // тест на кінець

    Turn operator *() const; // отримати хід

    void operator ++(); // наступний хід
};

#endif // TURNGENERATOR_H
