#include "match.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <magic.hpp>
#include <map>

#include "parsingtools.hpp"

struct TagsParserError {};
struct ImpossibleMoveError {};
struct LexicalParserError {};
struct UnknownParserError {};

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
  auto turns = SplitByDelims(line.substr(index), {' '});
  endboard_ = startboard_;
  for (auto turn : turns)
    if (!endboard_.ExecuteTurn(Turn::FromChessFormat(turn))) throw("error");
}

using Tags = std::unordered_map<std::string, std::string>;

Tags ParseTags(std::string_view data) {
  Tags tags;

  size_t index = 0;

  for (;;) {
    auto block_begin = data.find('[', index);
    auto block_end = data.find(']', block_begin);

    if (block_begin == std::string::npos || block_end == std::string::npos)
      break;

    auto sub_block = data.substr(block_begin, block_end - block_begin + 1);

    auto space = sub_block.find(' ', 0);
    assert(space != std::string::npos);
    auto tag = sub_block.substr(1, space - 1);
    auto value = sub_block.substr(space + 2, sub_block.size() - space - 4);

    tags[std::string(tag)] = std::string(value);

    index = block_end;
  }

  return tags;
}

std::string_view delim = "\n\n";

std::string_view ReadBlock(std::string_view data, size_t &index) {
  std::string_view result;

  auto block_index = data.find(delim, index + delim.size());
  if (block_index == std::string::npos) {
    result = data.substr(index, data.size() - index - delim.size());
    index = block_index;
  } else {
    if (block_index >= data.size() || block_index <= index)
      throw UnknownParserError();
    result = data.substr(index, block_index - index);
    index = block_index;
    while (index < data.size() && data[index] == '\n') index++;
    if (index == data.size()) index = std::string::npos;
  }
  return result;
}

Turn ParseAndExecuteTurn(std::string_view data, BitBoard &board) {
  if (data.size() < 2) throw LexicalParserError();

  const static std::map<char, Figure> bindf{{'N', Figure::kKnight},
                                            {'B', Figure::kBishop},
                                            {'R', Figure::kRook},
                                            {'Q', Figure::kQueen},
                                            {'K', Figure::kKing}};

  const static std::map<char, int> bindpx{{'a', 0}, {'b', 1}, {'c', 2},
                                          {'d', 3}, {'e', 4}, {'f', 5},
                                          {'g', 6}, {'h', 7}};

  const static std::map<char, int> bindpy{{'1', 7}, {'2', 6}, {'3', 5},
                                          {'4', 4}, {'5', 3}, {'6', 2},
                                          {'7', 1}, {'8', 0}};

  int from_x = -1;
  int from_y = -1;

  Figure figure = Figure::kPawn;
  Figure figure_transform = Figure::kEmpty;

  if (data.size() != 0 && data.back() == '+') {
    data.remove_suffix(1);
  }

  if (data.size() != 0 && data.back() == '#') {
    data.remove_suffix(1);
  }

  if (data == "O-O") {
    auto turn = Turn::GetShortCastling(board.CurrentColor());
    board.ExecuteTurn(turn);
    return turn;
  }
  if (data == "O-O-O") {
    auto turn = Turn::GetLongCastling(board.CurrentColor());
    board.ExecuteTurn(turn);
    return turn;
  }

  if (data.size() >= 2 && data[data.size() - 2] == '=') {
    try {
      figure_transform = bindf.at(data.back());
    } catch (std::out_of_range) {
      throw LexicalParserError();
    }
    data.remove_suffix(2);
  }

  auto to = Position::FromString(data.substr(data.size() - 2));
  if (!to.Valid()) throw LexicalParserError();
  data.remove_suffix(2);

  if (data.size() != 0 && data.back() == 'x') {
    data.remove_suffix(1);
  }

  if (data.size() != 0 && bindpy.count(data.back()) == 1) {
    try {
      from_y = bindpy.at(data.back());
    } catch (std::out_of_range) {
      throw LexicalParserError();
    }
    data.remove_suffix(1);
  }

  if (data.size() != 0 && bindpx.count(data.back()) == 1) {
    try {
      from_x = bindpx.at(data.back());
    } catch (std::out_of_range) {
      throw LexicalParserError();
    }
    data.remove_suffix(1);
  }

  if (data.size() != 0 && bindf.count(data.back()) == 1) {
    try {
      figure = bindf.at(data.back());
    } catch (std::out_of_range) {
      throw LexicalParserError();
    }
    data.remove_suffix(1);
  }

  bitboard_t from_mask = kall;
  bitboard_t to_mask = PositionToBitMask(to.Value());

  if (from_x != -1) from_mask &= rows[from_x];
  if (from_y != -1) from_mask &= lines[7 - from_y];

  from_mask &= board.GetBitBoard(board.CurrentColor(), figure);

  auto pboards =
      board.GenerateSubBoards(board.CurrentColor(), from_mask, to_mask);
  auto pturns = BitBoard::GenerateTurns(board, pboards, board.CurrentColor());

  if (pturns.size() == 1) {
    board = pboards.front();
    return pturns.front();
  } else {
    for (size_t i = 0; i < pturns.size(); ++i) {
      if (pturns[i].figure() == figure_transform) {
        board = pboards[i];
        return pturns[i];
      }
    }

    throw ImpossibleMoveError();
  }
}

std::pair<std::vector<Turn>, BitBoard> ParseTurns(std::string_view data,
                                                  BitBoard start_pos) {
  std::vector<std::string_view> turns;
  turns.reserve(100);
  std::vector<Turn> result;
  result.reserve(100);

  const static std::vector<char> seperators = {' ', '.', '\n'};
  std::vector<std::string_view> turns_pre = SplitByDelims(data, seperators);

  for (size_t i = 0; i < turns_pre.size(); i++)
    if (i % 3 != 0) turns.push_back(turns_pre[i]);

  BitBoard board(start_pos);
  for (auto turn_data : turns) {
    auto turn = ParseAndExecuteTurn(turn_data, board);
    result.push_back(turn);
  }
  return {result, board};
}

Match ReadMatch(std::string_view data, size_t &index) {
  auto header = ReadBlock(data, index);
  auto content = ReadBlock(data, index);

  auto tags = ParseTags(header);
  auto turns = ParseTurns(content, BitBoard());

  Match match;

  match.SetTurns(turns.first);
  match.SetBoard(turns.second);
  match.setEvent(tags.at("Event"));
  match.setSite(tags.at("Site"));
  match.setDate(tags.at("Date"));
  match.setRound(tags.at("Round"));
  match.setWhite(tags.at("White"));
  match.setBlack(tags.at("Black"));

  if (tags.at("Result") == "1-0")
    match.SetResult(Match::WhiteWin);
  else if (tags.at("Result") == "0-1")
    match.SetResult(Match::BlackWin);
  else if (tags.at("Result") == "1/2-1/2")
    match.SetResult(Match::Tie);
  else
    match.SetResult(Match::Unknown);

  return match;
}

std::vector<Match> LoadFromPGN(std::string text) {
  std::vector<Match> matches;
  size_t len = 0;
  while (len != std::string_view::npos) {
    try {
      matches.emplace_back(std::move(ReadMatch(text, len)));
    } catch (ImpossibleMoveError) {
      std::cout << "Round skipped immposible move" << std::endl;
    } catch (LexicalParserError) {
      std::cout << "Round skipped lexical error" << std::endl;
      return matches;
    }
  }
  return matches;
}
