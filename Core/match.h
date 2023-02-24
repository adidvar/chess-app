#ifndef MATCH_H
#define MATCH_H

#include <vector>

#include "board.h"
#include "turn.h"

class Match
{
    enum MatchState
    {
        kActiveNow, ///< Матч продовжується
        kWhiteCheckmate, ///< Білі зловили мат
        kBlackCheckmate, ///< Чорні зловили мат
        kTie, ///< Нічия
        kWhiteSurrendered, ///<Білі здалися
        kBlackSurrendered ///<Чорні здалися
    } state_;

    std::vector<Turn> turns_history_;
    std::vector<Board> boards_history_;
    std::vector<Turn> current_turns_; ///< захешовані комбінації ходів

protected:
    void UpdateState(); // обновляє стан і перехешовує ходи
public:
    Match(){};
    ~Match() =default;


    bool Push(Turn turn);
};

#endif // MATCH_H
