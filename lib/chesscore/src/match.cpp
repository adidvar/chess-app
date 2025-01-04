/*
#include "match.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>

#include "fen.hpp"

Match::Match() {}

Match::~Match() {}

bool Match::Push(Turn turn) {
  if (result_ != Unknown) return false;
  if (endboard_.ExecuteTurn(turn)) {
    turns_.push_back(turn);

    if (endboard_.WhiteWin())
      result_ = WhiteWin;
    else if (endboard_.BlackWin())
      result_ = BlackWin;
    else if (endboard_.Tie())
      result_ = Tie;

    return true;
  }
  return false;
}

bool Match::Surrender(Color color) {
  if (result_ == Unknown) {
    if (color == Color::White)
      result_ = BlackWin;
    else
      result_ = WhiteWin;
    return true;
  }
  return false;
}

std::vector<Turn> Match::GetTurns() const { return turns_; }

void Match::SetTurns(const std::vector<Turn> &turns) { turns_ = turns; }

BitBoard Match::GetBoard() const { return endboard_; }

void Match::SetBoard(const BitBoard &board) { endboard_ = board; }

BitBoard Match::GetStartBoard() const { return startboard_; }

void Match::SetStartBoard(const BitBoard &board) { startboard_ = board; }

void Match::LoadFromUCIString(const std::string &line) {
  size_t index = 0;
  loadFromFen(line, startboard_, index);
  auto turnsstr = line.substr(index);
  auto turns = splitLine(turnsstr, {' ', '\t', '\n', '\r'});
  endboard_ = startboard_;
  if (turns.size() != 0 && turns.front() == "moves") {
    turns.erase(turns.cbegin());
    for (std::string_view turnstr : turns) {
      auto turn = Turn::FromChessFormat(turnstr);
      if (!endboard_.ExecuteTurn(turn))
        throw std::runtime_error{"UCI string parsing error"};
    }
  }
}

*/
