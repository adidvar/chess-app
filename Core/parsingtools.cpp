#include "parsingtools.hpp"

#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>

#include "bitboard.hpp"

void LoadFromFen(std::string_view fen, BitBoard &board_, size_t &index) {
  std::string_view start = "startpos";
  auto miss =
      std::mismatch(fen.cbegin(), fen.cend(), start.cbegin(), start.cend());

  if (miss.second == start.cend()) {
    board_ = BitBoard();
    index = miss.second - start.cbegin();
    return;
  }

  for (size_t i = 0; i < Position::Max(); i++)
    board_.Set(i, {Figure::kEmpty, Color::kWhite});

  size_t i = index;
  size_t position = 0;
  while (i < fen.size() && position != 64 && fen[i] != ' ') {
    switch (fen.at(i)) {
      case 'p':
        board_.Set(position, {Figure::kPawn, Color::kBlack});
        position++;
        break;
      case 'n':
        board_.Set(position, {Figure::kKnight, Color::kBlack});
        position++;
        break;
      case 'b':
        board_.Set(position, {Figure::kBishop, Color::kBlack});
        position++;
        break;
      case 'r':
        board_.Set(position, {Figure::kRook, Color::kBlack});
        position++;
        break;
      case 'q':
        board_.Set(position, {Figure::kQueen, Color::kBlack});
        position++;
        break;
      case 'k':
        board_.Set(position, {Figure::kKing, Color::kBlack});
        position++;
        break;
      case 'P':
        board_.Set(position, {Figure::kPawn, Color::kWhite});
        position++;
        break;
      case 'N':
        board_.Set(position, {Figure::kKnight, Color::kWhite});
        position++;
        break;
      case 'B':
        board_.Set(position, {Figure::kBishop, Color::kWhite});
        position++;
        break;
      case 'R':
        board_.Set(position, {Figure::kRook, Color::kWhite});
        position++;
        break;
      case 'Q':
        board_.Set(position, {Figure::kQueen, Color::kWhite});
        position++;
        break;
      case 'K':
        board_.Set(position, {Figure::kKing, Color::kWhite});
        position++;
        break;
      case '1':
        position += 1;
        break;
      case '2':
        position += 2;
        break;
      case '3':
        position += 3;
        break;
      case '4':
        position += 4;
        break;
      case '5':
        position += 5;
        break;
      case '6':
        position += 6;
        break;
      case '7':
        position += 7;
        break;
      case '8':
        position += 8;
        break;
      case '/':
        position = ((position - 1) / 8) * 8 + 8;
        break;
      case ' ':
        break;
      default:
        throw std::runtime_error("fen invalid format [board]");
        break;
    }
    ++i;
  }
  ++i;

  if (position != 64) throw std::runtime_error("fen invalid format [board]");

  std::string current_move, rooking, pawn;
  std::string static_move, move_counter;

  current_move = ReadUntillDelims(fen, {' '}, i);
  rooking = ReadUntillDelims(fen, {' '}, i);
  pawn = ReadUntillDelims(fen, {' '}, i);

  static_move = ReadUntillDelims(fen, {' '}, i);
  move_counter = ReadUntillDelims(fen, {' '}, i);

  if (pawn.size() == 2) board_.SetLastPawnMove(Position::FromString(pawn));

  if (current_move.front() == 'w')
    board_.SetCurrentColor(Color::kWhite);
  else if (current_move == "b")
    board_.SetCurrentColor(Color::kBlack);
  else
    throw std::runtime_error("fen invalid format [current_move]");

  struct BitBoard::RookingFlagsT rooking_flags_ = {false, false, false, false};

  for (char x : rooking) {
    switch (x) {
      case 'K':
        rooking_flags_.white_oo = true;
        break;
      case 'Q':
        rooking_flags_.white_ooo = true;
        break;
      case 'k':
        rooking_flags_.black_oo = true;
        break;
      case 'q':
        rooking_flags_.black_ooo = true;
        break;
      case '-':
        break;
      default:
        throw std::runtime_error("fen invalid format [rooking]");
    }
  }
  board_.SetRookingFlags(rooking_flags_);
  index = i;
}

std::string SaveToFen(BitBoard board_) {
  const char *symbols[2] = {" PNBRQK", " pnbrqk"};
  char buffer[128] = {0};
  size_t position = 0;
  size_t bypass_counter = 0;
  for (size_t i = 0; i < 64; i++) {
    if (i % 8 == 0 && i != 0) {
      if (bypass_counter != 0) {
        buffer[position] = '0' + bypass_counter;
        position++;
        bypass_counter = 0;
      }

      buffer[position] = '/';
      position++;
    }
    if (board_.TestEmp(i))
      bypass_counter++;
    else {
      if (bypass_counter != 0) {
        buffer[position] = '0' + bypass_counter;
        position++;
        bypass_counter = 0;
      }
      buffer[position] = symbols[board_.GetColor(i)][board_.GetFigure(i)];
      position++;
    }
  }

  if (bypass_counter != 0) {
    buffer[position] = '0' + bypass_counter;
    position++;
  }

  std::string string(buffer);
  string += ' ';
  string.push_back(board_.CurrentColor() == Color::kWhite ? 'w' : 'b');
  string += ' ';
  auto rooking_flags = board_.RookingFlags();
  if (rooking_flags.white_oo == false && rooking_flags.white_ooo == false &&
      rooking_flags.black_oo == false && rooking_flags.black_ooo == false)
    string.push_back('-');
  else {
    if (rooking_flags.white_oo) string += 'K';
    if (rooking_flags.white_ooo) string += 'Q';
    if (rooking_flags.black_oo) string += 'k';
    if (rooking_flags.black_ooo) string += 'q';
  }
  string += ' ';

  if (board_.LastPawnMove().Valid())
    string += board_.LastPawnMove().ToString();
  else
    string += '-';
  string += ' ';
  string += std::to_string(0);
  string += ' ';
  string += std::to_string(0);
  return string;
}

std::string_view ReadUntillDelims(std::string_view data,
                                  const std::vector<char> &seperators,
                                  size_t &index) {
  auto prefix_size = index;
  data.remove_prefix(prefix_size);

  index = 0;

  while (std::find(seperators.begin(), seperators.end(), data[index]) !=
         seperators.end())
    index++;

  auto begin = index;

  while (index < data.size() && std::find(seperators.begin(), seperators.end(),
                                          data[index]) == seperators.end())
    index++;

  auto end = index;

  while (index < data.size() && std::find(seperators.begin(), seperators.end(),
                                          data[index]) != seperators.end())
    index++;

  index += prefix_size;

  return data.substr(begin, end - begin);
}

std::vector<std::string_view> SplitByDelims(
    std::string_view data, const std::vector<char> &seperators) {
  std::vector<std::string_view> parts;

  size_t index = 0;
  while (index < data.size())
    parts.push_back(ReadUntillDelims(data, seperators, index));

  return parts;
}
/*
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
*/

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

/*
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
*/

/*
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
*/
