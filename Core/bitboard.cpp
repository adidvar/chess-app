#include "bitboard.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>
#include <stdexcept>

#include "magic.hpp"
#include "parsingtools.hpp"
#include "zobrist.hpp"

const char *BitBoard::kStartPosition_ =
    u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const BitBoard BitBoard::kStartBitBoard_ = BitBoard(BitBoard::kStartPosition_);

BitBoard::BitBoard() : BitBoard(kStartBitBoard_) {}

BitBoard::BitBoard(std::string_view fen) : board_{{0}, {0}} {
  size_t index = 0;
  LoadFromFen(fen, *this, index);

  all_[Color::kWhite] = all_[Color::kBlack] = 0;

  for (size_t i = 1; i < Figure::Max(); i++) {
    all_[Color::kWhite] |= board_[Color::kWhite][i];
    all_[Color::kBlack] |= board_[Color::kBlack][i];
  }

  board_[Color::kWhite][Figure::kEmpty] =
      board_[Color::kBlack][Figure::kEmpty] =
          ~(all_[Color::kWhite] | all_[Color::kBlack]);
}

std::string BitBoard::Fen() const { return SaveToFen(*this); }

Color BitBoard::CurrentColor() const noexcept { return current_player_color_; }

Color BitBoard::OpponentColor() const noexcept {
  return !current_player_color_;
}

BitBoard::RookingFlagsT BitBoard::RookingFlags() const noexcept {
  return rooking_flags_;
}

Position BitBoard::LastPawnMove() const noexcept { return last_pawn_move_; }

void BitBoard::Set(Position position, Cell cell)  ///< Записує фігуру
{
  bitboard_t mask = PositionToBitMask(position);

  if (all_[Color::kWhite] & mask)
    for (size_t i = 1; i < Figure::Max(); i++)
      board_[Color::kWhite][i] &= ~mask;

  if (all_[Color::kBlack] & mask)
    for (size_t i = 1; i < Figure::Max(); i++)
      board_[Color::kBlack][i] &= ~mask;

  board_[cell.color][cell.type] |= mask;

  all_[Color::kWhite] = all_[Color::kBlack] = 0;

  for (size_t i = 1; i < Figure::Max(); i++) {
    all_[Color::kWhite] |= board_[Color::kWhite][i];
    all_[Color::kBlack] |= board_[Color::kBlack][i];
  }

  board_[Color::kWhite][Figure::kEmpty] =
      board_[Color::kBlack][Figure::kEmpty] =
          ~(all_[Color::kWhite] | all_[Color::kBlack]);
}

void BitBoard::SetCurrentColor(Color color) { current_player_color_ = color; }

void BitBoard::SetRookingFlags(RookingFlagsT flags) { rooking_flags_ = flags; }

void BitBoard::SetLastPawnMove(Position lp) { last_pawn_move_ = lp; }

void BitBoard::Swap(Position p1, Position p2) {
  assert(p1.Valid());
  assert(p2.Valid());

  auto cell = GetCell(p1);
  Set(p1, GetCell(p2));
  Set(p2, cell);
}

void BitBoard::SkipMove() { current_player_color_ = OpponentColor(); }

bool BitBoard::Test(Figure figure, Position position) const noexcept {
  assert(position.Valid());
  return ((board_[Color::kWhite][figure] | (board_[Color::kBlack][figure])) >>
          position.Value()) &
         1;
}

bool BitBoard::TestColor(Color color, Position position) const noexcept {
  assert(position.Valid());
  return (all_[color] >> position.Value()) & 1;
}

bool BitBoard::TestEmp(Position position) const noexcept {
  assert(position.Valid());
  return (board_[Color::kWhite][Figure::kEmpty] >> position.Value()) & 1;
}

BitBoard::Cell BitBoard::GetCell(Position position) const noexcept {
  assert(position.Valid());
  return {GetFigure(position), GetColor(position)};
}

bitboard_t BitBoard::GetBitBoard(Color color, Figure figure) const noexcept {
  return board_[color][figure];
}

bitboard_t BitBoard::GetColorBitBoard(Color color) const noexcept {
  return all_[color];
}

bool BitBoard::MateTest() const {
  return (AttackMask(OpponentColor()) &
          board_[CurrentColor()][Figure::kKing]) != 0;
}

bool BitBoard::End() const {
  return GenerateSubBoards(CurrentColor()).size() == 0;
}

bool BitBoard::Checkmate() const { return End() && MateTest(); }

bool BitBoard::WhiteWin() const {
  return (CurrentColor() == Color::kBlack) && End();
}

bool BitBoard::BlackWin() const {
  return (CurrentColor() == Color::kWhite) && End();
}

bool BitBoard::Tie() const { return End() && !MateTest(); }

bool BitBoard::OpponentMateTest() const {
  return (AttackMask(CurrentColor()) &
          board_[OpponentColor()][Figure::kKing]) != 0;
}

Figure BitBoard::GetFigure(Position position) const noexcept {
  assert(position.Valid());

  for (size_t i = 0; i < Figure::Max(); i++) {
    if (Test(i, position)) return (i);
  }

  return Figure::kEmpty;
}

Color BitBoard::GetColor(Position position) const noexcept {
  assert(position.Valid());
  return TestColor(Color::kBlack, position);
}

void BitBoard::Move(bitboard_t from, bitboard_t to, Color color, Figure type) {
  board_[color][type] &= ~from;
  board_[color][type] |= to;

  all_[color] &= ~from;
  all_[color] |= to;

  board_[Color::kWhite][Figure::kEmpty] =
      board_[Color::kBlack][Figure::kEmpty] =
          ~(all_[Color::kWhite] | all_[Color::kBlack]);

  if (from & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b)) {
    if (from & 0_b)
      rooking_flags_.black_ooo = false;
    else if (from & 4_b) {
      rooking_flags_.black_ooo = false;
      rooking_flags_.black_oo = false;
    } else if (from & 7_b)
      rooking_flags_.black_oo = false;
    else if (from & 56_b)
      rooking_flags_.white_ooo = false;
    else if (from & 60_b) {
      rooking_flags_.white_ooo = false;
      rooking_flags_.white_oo = false;
    } else if (from & 63_b)
      rooking_flags_.white_oo = false;
  }
}

void BitBoard::Attack(bitboard_t from, bitboard_t to, Color color,
                      Figure type) {
  board_[color][type] &= ~from;
  board_[color][type] |= to;

  all_[color] &= ~from;
  all_[color] |= to;

  for (size_t i = 1; i < 7; i++) board_[!color][i] &= ~to;
  all_[!color] &= ~to;

  board_[Color::kWhite][Figure::kEmpty] =
      board_[Color::kBlack][Figure::kEmpty] =
          ~(all_[Color::kWhite] | all_[Color::kBlack]);

  if (to & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b)) {
    if (to & 0_b)
      rooking_flags_.black_ooo = false;
    else if (to & 4_b) {
      rooking_flags_.black_ooo = false;
      rooking_flags_.black_oo = false;
    } else if (to & 7_b)
      rooking_flags_.black_oo = false;
    else if (to & 56_b)
      rooking_flags_.white_ooo = false;
    else if (to & 60_b) {
      rooking_flags_.white_ooo = false;
      rooking_flags_.white_oo = false;
    } else if (to & 63_b)
      rooking_flags_.white_oo = false;
  }
  if (from & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b)) {
    if (from & 0_b)
      rooking_flags_.black_ooo = false;
    else if (from & 4_b) {
      rooking_flags_.black_ooo = false;
      rooking_flags_.black_oo = false;
    } else if (from & 7_b)
      rooking_flags_.black_oo = false;
    else if (from & 56_b)
      rooking_flags_.white_ooo = false;
    else if (from & 60_b) {
      rooking_flags_.white_ooo = false;
      rooking_flags_.white_oo = false;
    } else if (from & 63_b)
      rooking_flags_.white_oo = false;
  }
}

void BitBoard::Transform(bitboard_t sq, Color color, Figure from, Figure to) {
  board_[color][from] &= ~sq;
  board_[color][to] |= sq;
}

struct Pawn {};

struct Knight {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
    return GenerateAttack(figure, all, allies) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    return (((figure << 10) & ~(row_a | row_b)) | ((figure << 17) & (~row_a)) |
            ((figure >> 6)) & ~(row_a | row_b) | ((figure >> 15) & ~(row_a)) |
            ((figure << 6) & ~(row_g | row_h)) | ((figure << 15) & ~(row_h)) |
            ((figure >> 10) & ~(row_g | row_h)) | ((figure >> 17) & ~(row_h)));
    ;
  }
  constexpr static auto type = Figure::kKnight;
};
struct King {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
    return GenerateAttack(figure, all, allies) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    return (((figure << 1) & ~row_a) | ((figure << 9) & ~row_a) |
            ((figure >> 7) & ~row_a) | (figure >> 8) |
            ((figure >> 1) & ~row_h) | ((figure >> 9) & ~row_h) |
            ((figure << 7) & ~row_h) | (figure << 8));
  }
  constexpr static auto type = Figure::kKing;
};
struct Bishop {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
    return ProcessBishop(figure, all) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
      mask |= ProcessBishop(iterator.Bit(), all);
      ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::kBishop;
};
struct Rook {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
    return ProcessRook(figure, all) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
      mask |= ProcessRook(iterator.Bit(), all);
      ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::kRook;
};
struct Queen {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
    return (ProcessBishop(figure, all) | ProcessRook(figure, all)) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
      mask |= (ProcessBishop(iterator.Bit(), all) |
               ProcessRook(iterator.Bit(), all));
      ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::kQueen;
};

template <typename Type>
void BitBoard::ProcessFigure(const BitBoard &parrent,
                             std::vector<BitBoard> &boards, Color color,
                             bitboard_t from_mask, bitboard_t to_mask,
                             bitboard_t all, bitboard_t yours,
                             bitboard_t opponent) const {
  bitboard_t map = board_[color][Type::type] & from_mask;
  BitIterator iterator(map);
  while (iterator.Valid()) {
    bitboard_t after = Type::Generate(iterator.Bit(), all, yours) & to_mask;
    after &= ~yours;
    bitboard_t attack = after & opponent;
    after &= ~attack;

    BitIterator to(after);
    bitboard_t reverse = ~iterator.Bit();
    while (to.Valid()) {
      boards.emplace_back(parrent);
      boards.back().Move(iterator.bit_, to.bit_, color, Type::type);

      ++to;
    }

    to = BitIterator(attack);
    while (to.Valid()) {
      boards.emplace_back(parrent);
      boards.back().Attack(iterator.bit_, to.bit_, color, Type::type);

      ++to;
    }
    ++iterator;
  }
}

template <>
void BitBoard::ProcessFigure<Pawn>(const BitBoard &parrent,
                                   std::vector<BitBoard> &boards, Color color,
                                   bitboard_t from_mask, bitboard_t to_mask,
                                   bitboard_t all, bitboard_t yours,
                                   bitboard_t opponent) const {
  bitboard_t map = board_[color][Figure::kPawn] & from_mask;

  bitboard_t possible;
  if (color == Color::kWhite)
    possible = (((map >> 8) & ~all) & to_mask) << 8;
  else
    possible = (((map << 8) & ~all) & to_mask) >> 8;

  BitIterator iterator(possible);
  while (iterator.Valid()) {
    bitboard_t after;
    if (color == Color::kWhite)
      after = iterator.Bit() >> 8;
    else
      after = iterator.Bit() << 8;

    BitBoard copy(parrent);
    copy.Move(iterator.Bit(), after, color, Figure::kPawn);
    boards.emplace_back(copy);
    if (after & (line_1 | line_8)) {
      boards.back().Transform(after, color, Figure::kPawn, Figure::kQueen);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kRook);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kBishop);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kKnight);
    }

    ++iterator;
  }

  if (color == Color::kWhite)
    possible = (((map & (~row_a)) >> (8 + 1)) & opponent & to_mask) << (8 + 1);
  else
    possible = (((map & (~row_h)) << (8 + 1)) & opponent & to_mask) >> (8 + 1);
  iterator = possible;

  while (iterator.Valid()) {
    bitboard_t after;
    if (color == Color::kWhite)
      after = iterator.Bit() >> (8 + 1);
    else
      after = iterator.Bit() << (8 + 1);

    BitBoard copy(parrent);
    copy.Attack(iterator.Bit(), after, color, Figure::kPawn);
    boards.emplace_back(copy);
    if (after & (line_1 | line_8)) {
      boards.back().Transform(after, color, Figure::kPawn, Figure::kQueen);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kRook);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kBishop);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kKnight);
    }

    ++iterator;
  }

  if (color == Color::kWhite)
    possible = (((map & (~row_h)) >> (8 - 1)) & opponent & to_mask) << (8 - 1);
  else
    possible = (((map & (~row_a)) << (8 - 1)) & opponent & to_mask) >> (8 - 1);

  iterator = possible;

  while (iterator.Valid()) {
    bitboard_t after;
    if (color == Color::kWhite)
      after = iterator.Bit() >> (8 - 1);
    else
      after = iterator.Bit() << (8 - 1);

    BitBoard copy(parrent);
    copy.Attack(iterator.Bit(), after, color, Figure::kPawn);
    boards.emplace_back(copy);
    if (after & (line_1 | line_8)) {
      boards.back().Transform(after, color, Figure::kPawn, Figure::kQueen);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kRook);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kBishop);
      boards.emplace_back(copy);
      boards.back().Transform(after, color, Figure::kPawn, Figure::kKnight);
    }

    ++iterator;
  }

  if (color == Color::kWhite)
    possible = (((((map & line_2) >> 8) & ~all) >> 8) & ~all & to_mask)
               << 2 * 8;
  else
    possible =
        (((((map & line_7) << 8) & ~all) << 8) & ~all & to_mask) >> 2 * 8;
  iterator = possible;

  while (iterator.Valid()) {
    bitboard_t after;
    bitboard_t el_passant;
    if (color == Color::kWhite) {
      after = iterator.Bit() >> 2 * 8;
      el_passant = iterator.Bit() >> 8;
    } else {
      after = iterator.Bit() << 2 * 8;
      el_passant = iterator.Bit() << 8;
    }

    boards.emplace_back(parrent);
    boards.back().last_pawn_move_ = log2_64(el_passant);
    boards.back().Move(iterator.Bit(), after, color, Figure::kPawn);

    ++iterator;
  }
}

template <typename Type>
bitboard_t BitBoard::ProcessAttack(Color color, bitboard_t from_mask,
                                   bitboard_t all, bitboard_t yours,
                                   bitboard_t opponent) const {
  bitboard_t map = board_[color][Type::type] & from_mask;
  return Type::GenerateAttack(map, all, yours);
}

template <>
bitboard_t BitBoard::ProcessAttack<Pawn>(Color color, bitboard_t from_mask,
                                         bitboard_t all, bitboard_t yours,
                                         bitboard_t opponent) const {
  bitboard_t map = board_[color][Figure::kPawn] & from_mask;

  if (color == Color::kWhite) {
    return (((map & (~row_a)) >> (8 + 1))) | (((map & (~row_h)) >> (8 - 1)));
  } else {
    return (((map & (~row_h)) << (8 + 1))) | (((map & (~row_a)) << (8 - 1)));
  }
}

bitboard_t BitBoard::AttackMask(Color color) const {
  bitboard_t empty = board_[color][Figure::kEmpty];
  bitboard_t opponent = all_[!color];
  bitboard_t yours = all_[color];
  bitboard_t all = ~empty;

  bitboard_t result = 0;

  // pawns
  result |= ProcessAttack<Pawn>(color, kall, all, yours, opponent);
  // knight
  result |= ProcessAttack<Knight>(color, kall, all, yours, opponent);
  // king
  result |= ProcessAttack<King>(color, kall, all, yours, opponent);
  // bishop
  result |= ProcessAttack<Bishop>(color, kall, all, yours, opponent);
  // rook
  result |= ProcessAttack<Rook>(color, kall, all, yours, opponent);
  // queen
  result |= ProcessAttack<Queen>(color, kall, all, yours, opponent);

  return result;
}

void BitBoard::GenerateSubBoards(Color color, std::vector<BitBoard> &boards,
                                 uint64_t from, uint64_t to) const {
  boards.clear();

  bitboard_t empty = board_[color][Figure::kEmpty];
  bitboard_t opponent = all_[!color];
  bitboard_t yours = all_[color];
  bitboard_t all = ~empty;
  bitboard_t attack = AttackMask(!color);

  BitBoard parent(*this);
  parent.last_pawn_move_ = Position();
  parent.SkipMove();

  bitboard_t locked_figures =
      attack & yours & AttackFrom(board_[color][Figure::kKing]);

  // queen
  ProcessFigure<Queen>(parent, boards, color, ~locked_figures & from, to, all,
                       yours, opponent);
  // rook
  ProcessFigure<Rook>(parent, boards, color, ~locked_figures & from, to, all,
                      yours, opponent);
  // knight
  ProcessFigure<Knight>(parent, boards, color, ~locked_figures & from, to, all,
                        yours, opponent);
  // bishop
  ProcessFigure<Bishop>(parent, boards, color, ~locked_figures & from, to, all,
                        yours, opponent);
  // pawns
  ProcessFigure<Pawn>(parent, boards, color, ~locked_figures & from, to, all,
                      yours, opponent);
  // king
  ProcessFigure<King>(parent, boards, color, ~locked_figures & from,
                      ~attack & to, all, yours, opponent);

  auto index = boards.size();
  if ((attack & board_[color][Figure::kKing]) != 0) index = 0;

  // queen
  ProcessFigure<Queen>(parent, boards, color, locked_figures & from, to, all,
                       yours, opponent);
  // rook
  ProcessFigure<Rook>(parent, boards, color, locked_figures & from, to, all,
                      yours, opponent);
  // knight
  ProcessFigure<Knight>(parent, boards, color, locked_figures & from, to, all,
                        yours, opponent);
  // bishop
  ProcessFigure<Bishop>(parent, boards, color, locked_figures & from, to, all,
                        yours, opponent);
  // pawns
  ProcessFigure<Pawn>(parent, boards, color, locked_figures & from, to, all,
                      yours, opponent);
  // king
  ProcessFigure<King>(parent, boards, color, locked_figures & from,
                      ~attack & to, all, yours, opponent);

  if (last_pawn_move_.Valid() && CurrentColor() == color) {
    bitboard_t sq = 1ULL << last_pawn_move_.Value();
    bitboard_t pawns = board_[color][Figure::kPawn];
    if (color == Color::kWhite) {
      bitboard_t possible = ((((pawns & from & (~row_a)) >> (8 + 1)) & sq & to))
                            << (8 + 1);
      if (possible) {
        boards.emplace_back(parent);
        boards.back().Attack(possible, sq << 8, color, Figure::kPawn);
        boards.back().Move(sq << 8, sq, color, Figure::kPawn);
      }
      possible = ((((pawns & from & (~row_h)) >> (8 - 1)) & sq & to))
                 << (8 - 1);
      if (possible) {
        boards.emplace_back(parent);
        boards.back().Attack(possible, sq << 8, color, Figure::kPawn);
        boards.back().Move(sq << 8, sq, color, Figure::kPawn);
      }
    } else {
      bitboard_t possible =
          ((((pawns & from & (~row_h)) << (8 + 1)) & sq & to)) >> (8 + 1);
      if (possible) {
        boards.emplace_back(parent);
        boards.back().Attack(possible, sq >> 8, color, Figure::kPawn);
        boards.back().Move(sq >> 8, sq, color, Figure::kPawn);
      }
      possible =
          ((((pawns & from & (~row_a)) << (8 - 1)) & sq & to)) >> (8 - 1);
      if (possible) {
        boards.emplace_back(parent);
        boards.back().Attack(possible, sq >> 8, color, Figure::kPawn);
        boards.back().Move(sq >> 8, sq, color, Figure::kPawn);
      }
    }
  }

  auto it =
      std::remove_if(boards.begin() + index, boards.end(),
                     [](const BitBoard &b) { return b.OpponentMateTest(); });
  boards.erase(it, boards.end());

  // rooking
  if (color == Color::kWhite) {
    bitboard_t right = rooking_flags_.white_oo;
    right = (right << 63) & board_[Color::kWhite][Figure::kRook];
    right >>= 1;
    right &= (empty & ~attack) & to;
    right >>= 1;
    right &= (empty & ~attack);
    right >>= 1;
    right &= (~attack) & board_[Color::kWhite][Figure::kKing] & from;

    bitboard_t left = rooking_flags_.white_ooo;
    left = (left << 56) & board_[Color::kWhite][Figure::kRook];
    left <<= 1;
    left &= empty;
    left <<= 1;
    left &= (empty & ~attack) & to;
    left <<= 1;
    left &= (empty & ~attack);
    left <<= 1;
    left &= (~attack) & board_[Color::kWhite][Figure::kKing] & from;

    if (right != 0) {
      boards.emplace_back(parent);
      boards.back().Move(1ULL << 60, 1ULL << 62, Color::kWhite, Figure::kKing);
      boards.back().Move(1ULL << 63, 1ULL << 61, Color::kWhite, Figure::kRook);
      boards.back().rooking_flags_.white_oo = false;
      boards.back().rooking_flags_.white_ooo = false;
    }
    if (left != 0) {
      boards.emplace_back(parent);
      boards.back().Move(1ULL << 60, 1ULL << 58, Color::kWhite, Figure::kKing);
      boards.back().Move(1ULL << 56, 1ULL << 59, Color::kWhite, Figure::kRook);
      boards.back().rooking_flags_.white_oo = false;
      boards.back().rooking_flags_.white_ooo = false;
    }
  } else {
    bitboard_t right = rooking_flags_.black_oo;
    right = (right << 7) & board_[Color::kBlack][Figure::kRook];
    right >>= 1;
    right &= (empty & ~attack) & to;
    right >>= 1;
    right &= (empty & ~attack);
    right >>= 1;
    right &= (~attack) & board_[Color::kBlack][Figure::kKing] & from;

    bitboard_t left = rooking_flags_.black_ooo;
    left = (left << 0) & board_[Color::kBlack][Figure::kRook];
    left <<= 1;
    left &= empty;
    left <<= 1;
    left &= (empty & ~attack) & to;
    left <<= 1;
    left &= (empty & ~attack);
    left <<= 1;
    left &= (~attack) & board_[Color::kBlack][Figure::kKing] & from;

    if (right != 0) {
      boards.emplace_back(parent);
      boards.back().Move(1ULL << 4, 1ULL << 6, Color::kBlack, Figure::kKing);
      boards.back().Move(1ULL << 7, 1ULL << 5, Color::kBlack, Figure::kRook);
      boards.back().rooking_flags_.black_oo = false;
      boards.back().rooking_flags_.black_ooo = false;
    }
    if (left != 0) {
      boards.emplace_back(parent);
      boards.back().Move(1ULL << 4, 1ULL << 2, Color::kBlack, Figure::kKing);
      boards.back().Move(1ULL << 0, 1ULL << 3, Color::kBlack, Figure::kRook);
      boards.back().rooking_flags_.black_oo = false;
      boards.back().rooking_flags_.black_ooo = false;
    }
  }
}

std::vector<BitBoard> BitBoard::GenerateSubBoards(Color color, uint64_t from,
                                                  uint64_t to) const {
  std::vector<BitBoard> boards;
  boards.reserve(120);

  GenerateSubBoards(color, boards, from, to);
  return boards;
}

void BitBoard::GenerateSubBoards(std::vector<BitBoard> &boards, Color color,
                                 uint64_t from, uint64_t to) const {
  GenerateSubBoards(color, boards, from, to);
}

std::vector<Turn> BitBoard::GenerateTurns(Color color, uint64_t from,
                                          uint64_t to) const {
  std::vector<BitBoard> boards;
  std::vector<Turn> turns;
  turns.reserve(120);
  boards.reserve(120);
  GenerateSubBoards(boards, color, from, to);

  for (const auto &subboard : boards)
    turns.push_back(GetTurn(*this, subboard, color));
  return turns;
}

bool BitBoard::ExecuteTurn(Turn turn) {
  auto maps =
      GenerateSubBoards(CurrentColor(), operator""_b(turn.from().Value()),
                                        operator""_b(turn.to().Value()));
  auto turns = BitBoard::GenerateTurns(*this, maps, CurrentColor());
  for (size_t i = 0; i < turns.size(); ++i) {
    if (turns[i] == turn) {
      *this = maps[i];
      return true;
    }
  }
  return false;
}

bool BitBoard::TestTurn(Turn turn) const {
  auto turns = GenerateTurns(CurrentColor());
  return std::count(turns.begin(), turns.end(), turn) == 1;
}

bitboard_hash_t BitBoard::Hash() const {
  bitboard_hash_t hash = 0;
  /*
bitboard_t GetFigureHash(Figure figure, Color color, Position position);
bitboard_t GetShortCastlingFlagHash(Color color);
bitboard_t GetLongCastlingFlagHash(Color color);
bitboard_t GetCurrentColorHash(Color color);
bitboard_t GetElpassantHash(Position position);
*/

  for (size_t i = 0; i < 64; i++) {
    if (!TestEmp(i)) hash ^= GetFigureHash(GetFigure(i), GetColor(i), i);
  }

  return hash;
}

std::vector<BitBoardTuple> BitBoard::GenerateTuplesFast(BitBoardTuple tuple,
                                                        Color color,
                                                        uint64_t from,
                                                        uint64_t to) {
  auto boards = tuple.board.GenerateSubBoards(color, from, to);

  std::vector<BitBoardTuple> tuples;
  tuples.reserve(boards.size());

  for (auto &board : boards) {
    auto turn = GetTurn(tuple.board, board, color);
    tuples.push_back(
        {board, GetHash(tuple.board, tuple.hash, turn, board), turn});
  }

  return tuples;
}

Turn BitBoard::GetTurn(const BitBoard &board, const BitBoard &subboard,
                       Color color) {
  bitboard_t delta = board.all_[color] ^ subboard.all_[color];
  bitboard_t from = board.all_[color] & delta;
  bitboard_t to = subboard.all_[color] & delta;

  if (krooking_masks[color][0] == delta) {
    return Turn::GetLongCastling(color);
  } else if (krooking_masks[color][1] == delta) {
    return Turn::GetShortCastling(color);
  } else if ((to & (line_1 | line_8)) != 0 &&
             (from & board.board_[color][Figure::kPawn]) != 0) {
    return Turn(log2_64(from), log2_64(to), subboard.GetFigure(log2_64(to)));
  } else {
    return Turn(log2_64(from), log2_64(to));
  }
}

bitboard_hash_t BitBoard::GetHash(const BitBoard &board, bitboard_hash_t hash,
                                  Turn turn, const BitBoard &sub) {
  auto from = board.GetCell(turn.from());
  auto to = board.GetCell(turn.to());
  hash ^= GetFigureHash(from.type,from.color,turn.from());
  if(to.type != Figure::kEmpty)
      hash ^= GetFigureHash(to.type,to.color,turn.to());
  hash ^= GetFigureHash(from.type,from.color,turn.to());

  if (from.type == Figure::kPawn && to.type == Figure::kEmpty &&
      turn.from().x() != turn.to().x()) {
      return sub.Hash();
  }
  if (turn.IsCastling()) {
      return sub.Hash();
  }
  if (turn.IsTrasformation()) {
      return sub.Hash();
  }

  return hash;
}

std::vector<Turn> BitBoard::GenerateTurns(
    const BitBoard &main, const std::vector<BitBoard> &subboards, Color color) {
  std::vector<Turn> turns;
  turns.reserve(subboards.size());

  for (const auto &subboard : subboards)
    turns.push_back(GetTurn(main, subboard, color));
  return turns;
}

bool BitBoard::operator==(const BitBoard &board) const {
  if (rooking_flags_.black_oo != board.rooking_flags_.black_oo) return false;

  if (rooking_flags_.black_ooo != board.rooking_flags_.black_ooo) return false;

  if (rooking_flags_.white_oo != board.rooking_flags_.white_oo) return false;

  if (rooking_flags_.white_ooo != board.rooking_flags_.white_ooo) return false;

  if (last_pawn_move_ != last_pawn_move_) return false;

  for (size_t color = 0; color < Color::Max(); color++)
    for (size_t figure = 0; figure < Figure::Max(); figure++)
      if (board_[color][figure] != board.board_[color][figure]) return false;

  return true;
}

bool BitBoard::operator!=(const BitBoard &board) const {
  return !(*this == board);
}
