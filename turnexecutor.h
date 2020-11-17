#ifndef TURNEXECUTOR_H
#define TURNEXECUTOR_H
#include "turn.h"
#include "board.h"
#include "turngenerator.h"

class TurnExecutor
{
public:
    static void ExecuteTurn(Board &board , Turn turn);
    static bool ExecuteTurnSafe(Board &board , Turn turn);
    static bool TurnTest(const Board& board, const Turn);
};

#endif // TURNEXECUTOR_H
