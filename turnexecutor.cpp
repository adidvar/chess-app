#include "turnexecutor.h"

void TurnExecutor::ExecuteTurn(Board &board, Turn turn)
{
    switch (turn.type) {
    case Move:
        board.Set(turn.position_2,board.GetFigure(turn.position_1),board.GetColor(turn.position_1));
        board.Set(turn.position_1,empty,white);
        break;

    case PawnMove:
        board.Set(turn.position_2,turn.figure_type,board.GetColor(turn.position_1));
        board.Set(turn.position_1,empty,white);
        break;
    case Rooking:
        //рокировки

        break;
    case ErrorMove:
        throw std::runtime_error("try to do ErrorMove");
    default:
        return;
    }
    board.Rotate();
}

bool TurnExecutor::TurnTest(const Board &board, const Turn turn)
{
    Board sub(board);
    for(TurnGenerator gen(board);gen;++gen)
    {
        if( (*gen) == turn)
            return true;
    }
    return false;
}
