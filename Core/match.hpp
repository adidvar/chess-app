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

  bool Push(Turn turn);
  bool Surrender(Color color);

  std::vector<Turn> GetTurns() const;
  void SetTurns(const std::vector<Turn> &turns);
  BitBoard GetBoard() const;
  void SetBoard(const BitBoard &board);
  BitBoard GetStartBoard() const;
  void SetStartBoard(const BitBoard &board);

  bool HaveOpTag(const std::string &name) const;
  std::string GetOpTagValue(const std::string &name) const;
  void AddOpTag(const std::string &name, const std::string &value);

  std::string Event() const;
  void setEvent(const std::string &newEvent);

  std::string Site() const;
  void setSite(const std::string &newSite);

  std::string Date() const;
  void setDate(const std::string &newDate);

  std::string Round() const;
  void setRound(const std::string &newRound);

  std::string White() const;
  void setWhite(const std::string &newWhite);

  std::string Black() const;
  void setBlack(const std::string &newBlack);

  Result_t GetResult() const;
  void SetResult(Result_t value);

  void LoadFromUCIString(const std::string &line);

 private:
  std::string event_;
  std::string site_;
  std::string date_;
  std::string round_;
  std::string white_;
  std::string black_;
  Result_t result_ = Unknown;

  std::unordered_map<std::string, std::string> optags_;

  std::vector<Turn> turns_;
  BitBoard startboard_;
  BitBoard endboard_;
};

std::vector<Match> LoadFromPGN(std::string text);

#endif  // MATCH_H
