#include "computer.hpp"
#include "alphabeta.hpp"

Computer::Computer(Match &match, Color color):
    match_(match),
    table_(),
    color_(color)
{
}

Computer::~Computer()
{
}

void Computer::Start()
{
}

void Computer::Wait()
{
}

bool Computer::Ready()
{
    return false;
}

Turn Computer::GetBestTurn()
{
    table_.Clear();
    assert(color_ == match_.GetBoard().CurrentColor());
    AlphaBeta<MainAppraiser> ab(color_,stat_, table_);
    return ab.GetBestTurn(match_.GetBoard(), 5);
}

void Computer::LoadMarks(std::vector<std::pair<MainAppraiser, Turn>> &turns)
{
    AlphaBeta<MainAppraiser> ab(color_,stat_, table_);

    for(Turn turn : match_.GetBoard().GenerateTurns(match_.GetBoard().CurrentColor())){
        auto subboard = match_.GetBoard();
        subboard.ExecuteTurn(turn);
        turns.push_back({ab.GetValue(subboard,0),turn});
    }
}

Statistics Computer::GetStatistics()
{
    return stat_;
}
