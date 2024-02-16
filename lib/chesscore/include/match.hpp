#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <unordered_map>

#include "bitboard.hpp"
#include "turn.hpp"

class Match {
 public:
  enum Result_t { WhiteWin, BlackWin, Tie, Unknown };

  Match();
  Match(const Match &) = default;
  Match(Match &&from) = default;
  ~Match();

  Match &operator=(const Match &match) = default;

  bool Push(Turn turn);
  bool Surrender(Color color);

  std::vector<Turn> GetTurns() const;
  void SetTurns(const std::vector<Turn> &turns);
  BitBoard GetBoard() const;
  void SetBoard(const BitBoard &board);
  BitBoard GetStartBoard() const;
  void SetStartBoard(const BitBoard &board);
  void LoadFromUCIString(const std::string &line);

 private:
  Result_t result_ = Unknown;
  std::vector<Turn> turns_;
  BitBoard startboard_;
  BitBoard endboard_;
};

#endif  // MATCH_H
