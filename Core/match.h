#ifndef MATCH_H
#define MATCH_H

#include <vector>

#include "board.h"
#include "turn.h"
/*
class Match
{
    enum MatchState
    {
        active_now = 0, ///< Матч продовжується
        white_checkmate, ///< Білі зловили мат
        black_checkmate, ///< Чорні зловили мат
        tie, ///< Нічия
        white_surrendered, ///<Білі здалися
        black_surrendered ///<Чорні здалися
    } state;

    std::vector<Turn> turns_history;
    std::vector<Board> boards_history;
    std::vector<Turn> current_turns; ///< захешовані комбінації ходів

protected:
    void update_state_and_hash_turns(); // обновляє стан і перехешовує ходи


public:
    Match() =default;
    ~Match() =default;

    bool IsEnd() const {return state != active_now;};
    bool IsCheckmate() const {return state == white_checkmate || state == black_checkmate;};
    bool WhiteWin() const {return state == black_surrendered || state == black_checkmate;};
    bool BlackWin() const {return state == white_surrendered || state == white_checkmate;};
    bool Tie() const {return state == tie;};

    bool Push(Turn turn);
};
*/

#endif // MATCH_H
