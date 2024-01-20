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
    if (color == Color::kWhite)
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

bool Match::HaveOpTag(const std::string &name) const {
  return optags_.count(name) != 0;
}

std::string Match::GetOpTagValue(const std::string &name) const {
  return optags_.at(name);
}

void Match::AddOpTag(const std::string &name, const std::string &value) {
  optags_[name] = value;
}

std::string Match::Event() const { return event_; }

void Match::setEvent(const std::string &newEvent) { event_ = newEvent; }

std::string Match::Site() const { return site_; }

void Match::setSite(const std::string &newSite) { site_ = newSite; }

std::string Match::Date() const { return date_; }

void Match::setDate(const std::string &newDate) { date_ = newDate; }

std::string Match::Round() const { return round_; }

void Match::setRound(const std::string &newRound) { round_ = newRound; }

std::string Match::White() const { return white_; }

void Match::setWhite(const std::string &newWhite) { white_ = newWhite; }

std::string Match::Black() const { return black_; }

void Match::setBlack(const std::string &newBlack) { black_ = newBlack; }

Match::Result_t Match::GetResult() const { return result_; }

void Match::SetResult(Result_t value) { result_ = value; }

void Match::LoadFromUCIString(const std::string &line) {
  size_t index = 0;
  LoadFromFen(line, startboard_, index);
  auto turnsstr = line.substr(index);
  auto turns = Split(turnsstr, {' ', '\t', '\n', '\r'});
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
