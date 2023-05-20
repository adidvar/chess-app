#ifndef MATCH_H
#define MATCH_H

#include <vector>
#include <string>

#include "board.hpp"
#include "bitboard.hpp"
#include "turn.hpp"

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

    std::vector<Turn> turns_;
    BitBoard current_;

    std::vector<std::pair<std::string,std::string>> tags_;
public:
    Match(){};
    ~Match() =default;

    bool Push(Turn turn);
    bool Surrender(Color color);

    std::vector<Turn> GetTurns() const;
    BitBoard GetBoard() const;

    bool HaveTag(std::string name) const;
    std::string GetTagValue(std::string name) const;
};

std::vector<Match> LoadFromPGN(std::string text);

#endif // MATCH_H
