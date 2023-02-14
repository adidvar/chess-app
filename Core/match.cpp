#include "match.h"
#include "turnexecutor.h"
#include "turngenerator.h"

void Match::update_state_and_hash_turns()
{
    this->current_turns = TurnGenerate(boards_history.back(),boards_history.back().CurrentColor());
}

bool Match::Push(Turn turn)
{
    if(!TurnTest(boards_history.back(),turn))
        return false;

    this->turns_history.push_back(turn);
    Board copy = boards_history.back();
    ExecuteTurn(copy,turn);
    this->boards_history.push_back(std::move(copy));

    update_state_and_hash_turns();
    return true;
}
