#include "bitboard.hpp"

#include "bitutils.hpp"
#include "fen.hpp"
#include "magic.hpp"

const char *const BitBoard::kStartPosition
    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const BitBoard BitBoard::kStartBitBoard{BitBoard::kStartPosition};

BitBoard::BitBoard() : BitBoard(kStartBitBoard) {}

BitBoard::BitBoard(std::string_view fen)
{
    size_t index = 0;
    boardFromFen(fen, *this, index);
}

std::string BitBoard::fen() const
{
    return boardToFen(*this);
}
/*
void BitBoard::Move(bitboard from, bitboard to, Color color, Figure type) {
  board_[color][type] &= ~from;
  board_[color][type] |= to;

  all_[color] &= ~from;
  all_[color] |= to;

  board_[Color::White][Figure::Empty] = board_[Color::Black][Figure::Empty] = ~(
      all_[Color::White] | all_[Color::Black]);

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

void BitBoard::Attack(bitboard from, bitboard to, Color color,
                      Figure type) {
  board_[color][type] &= ~from;
  board_[color][type] |= to;

  all_[color] &= ~from;
  all_[color] |= to;

  for (size_t i = 1; i < 7; i++) board_[!color][i] &= ~to;
  all_[!color] &= ~to;

  board_[Color::White][Figure::Empty] = board_[Color::Black][Figure::Empty] = ~(
      all_[Color::White] | all_[Color::Black]);

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

void BitBoard::Transform(bitboard sq, Color color, Figure from, Figure to) {
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
            (((figure >> 6)) & ~(row_a | row_b)) | ((figure >> 15) & ~(row_a)) |
            ((figure << 6) & ~(row_g | row_h)) | ((figure << 15) & ~(row_h)) |
            ((figure >> 10) & ~(row_g | row_h)) | ((figure >> 17) & ~(row_h)));
    ;
  }
  constexpr static auto type = Figure::WKnight;
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
  constexpr static auto type = Figure::WKing;
};
struct Bishop {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
      return processBishop(figure, all) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
        mask |= processBishop(iterator.Bit(), all);
        ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::WBishop;
};
struct Rook {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
      return processRook(figure, all) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
        mask |= processRook(iterator.Bit(), all);
        ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::WRook;
};
struct Queen {
  static bitboard_t Generate(bitboard_t figure, bitboard_t all,
                             bitboard_t allies) {
      return (processBishop(figure, all) | processRook(figure, all)) & ~allies;
  }
  static bitboard_t GenerateAttack(bitboard_t figure, bitboard_t all,
                                   bitboard_t allies) {
    bitboard_t mask = 0;
    BitIterator iterator(figure);
    while (iterator.Valid()) {
        mask |= (processBishop(iterator.Bit(), all) | processRook(iterator.Bit(), all));
        ++iterator;
    }
    return mask;
  }
  constexpr static auto type = Figure::WQueen;
};

template <typename Type>
void BitBoard::ProcessFigure(const BitBoard &parrent,
                             std::vector<BitBoard> &boards, Color color,
                             bitboard from_mask, bitboard to_mask,
                             bitboard all, bitboard yours,
                             bitboard opponent) const {
  bitboard map = board_[color][Type::type] & from_mask;
  BitIterator iterator(map);
  while (iterator.Valid()) {
    bitboard after = Type::Generate(iterator.Bit(), all, yours) & to_mask;
    after &= ~yours;
    bitboard attack = after & opponent;
    after &= ~attack;

    BitIterator to(after);
    bitboard reverse = ~iterator.Bit();
    while (to.Valid()) {
      boards.emplace_back(parrent);
      boards.back().Move(iterator.Bit(), to.Bit(), color, Type::type);

      ++to;
    }

    to = BitIterator(attack);
    while (to.Valid()) {
      boards.emplace_back(parrent);
      boards.back().Attack(iterator.Bit(), to.Bit(), color, Type::type);

      ++to;
    }
    ++iterator;
  }
}

template <>
void BitBoard::ProcessFigure<Pawn>(const BitBoard &parrent,
                                   std::vector<BitBoard> &boards, Color color,
                                   bitboard from_mask, bitboard to_mask,
                                   bitboard all, bitboard yours,
                                   bitboard opponent) const {
    bitboard map = board_[color][Figure::WPawn] & from_mask;

    bitboard possible;
    if (color == Color::White)
        possible = (((map >> 8) & ~all) & to_mask) << 8;
    else
        possible = (((map << 8) & ~all) & to_mask) >> 8;

    BitIterator iterator(possible);
    while (iterator.Valid()) {
        bitboard after;
        if (color == Color::White)
            after = iterator.Bit() >> 8;
        else
            after = iterator.Bit() << 8;

        BitBoard copy(parrent);
        copy.Move(iterator.Bit(), after, color, Figure::WPawn);
        boards.emplace_back(copy);
        if (after & (line_1 | line_8)) {
            boards.back().Transform(after, color, Figure::WPawn, Figure::WQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WRook);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WKnight);
        }

        ++iterator;
    }

    if (color == Color::White)
        possible = (((map & (~row_a)) >> (8 + 1)) & opponent & to_mask) << (8 + 1);
    else
        possible = (((map & (~row_h)) << (8 + 1)) & opponent & to_mask) >> (8 + 1);
    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        if (color == Color::White)
            after = iterator.Bit() >> (8 + 1);
        else
            after = iterator.Bit() << (8 + 1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(), after, color, Figure::WPawn);
        boards.emplace_back(copy);
        if (after & (line_1 | line_8)) {
            boards.back().Transform(after, color, Figure::WPawn, Figure::WQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WRook);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WKnight);
        }

        ++iterator;
    }

    if (color == Color::White)
        possible = (((map & (~row_h)) >> (8 - 1)) & opponent & to_mask) << (8 - 1);
    else
        possible = (((map & (~row_a)) << (8 - 1)) & opponent & to_mask) >> (8 - 1);

    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        if (color == Color::White)
            after = iterator.Bit() >> (8 - 1);
        else
            after = iterator.Bit() << (8 - 1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(), after, color, Figure::WPawn);
        boards.emplace_back(copy);
        if (after & (line_1 | line_8)) {
            boards.back().Transform(after, color, Figure::WPawn, Figure::WQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WRook);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WKnight);
        }

        ++iterator;
    }

    if (color == Color::White)
        possible = (((((map & line_2) >> 8) & ~all) >> 8) & ~all & to_mask) << 2 * 8;
    else
        possible = (((((map & line_7) << 8) & ~all) << 8) & ~all & to_mask) >> 2 * 8;
    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        bitboard el_passant;
        if (color == Color::White) {
            after = iterator.Bit() >> 2 * 8;
            el_passant = iterator.Bit() >> 8;
        } else {
            after = iterator.Bit() << 2 * 8;
            el_passant = iterator.Bit() << 8;
        }

        boards.emplace_back(parrent);
        boards.back().last_pawn_move_ = log2_64(el_passant);
        boards.back().Move(iterator.Bit(), after, color, Figure::WPawn);

        ++iterator;
    }
}

template <typename Type>
bitboard_t BitBoard::ProcessAttack(Color color, bitboard from_mask,
                                   bitboard all, bitboard yours,
                                   bitboard opponent) const {
  bitboard map = board_[color][Type::type] & from_mask;
  return Type::GenerateAttack(map, all, yours);
}

template <>
bitboard_t BitBoard::ProcessAttack<Pawn>(Color color, bitboard from_mask,
                                         bitboard all, bitboard yours,
                                         bitboard opponent) const {
    bitboard map = board_[color][Figure::WPawn] & from_mask;

    if (color == Color::White) {
        return (((map & (~row_a)) >> (8 + 1))) | (((map & (~row_h)) >> (8 - 1)));
    } else {
        return (((map & (~row_h)) << (8 + 1))) | (((map & (~row_a)) << (8 - 1)));
    }
}

bitboard_t BitBoard::AttackMask(Color color, bitboard from) const {
    bitboard empty = board_[color][Figure::Empty];
    bitboard opponent = all_[!color];
    bitboard yours = all_[color];
    bitboard all = ~empty;

    bitboard result = 0;

    // pawns
    result |= ProcessAttack<Pawn>(color, from, all, yours, opponent);
    // knight
    result |= ProcessAttack<Knight>(color, from, all, yours, opponent);
    // king
    result |= ProcessAttack<King>(color, from, all, yours, opponent);
    // bishop
    result |= ProcessAttack<Bishop>(color, from, all, yours, opponent);
    // rook
    result |= ProcessAttack<Rook>(color, from, all, yours, opponent);
    // queen
    result |= ProcessAttack<Queen>(color, from, all, yours, opponent);

    return result;
}

void BitBoard::generateSubBoards(Color color, std::vector<BitBoard> &boards,
                                 uint64_t from, uint64_t to) const {
  boards.clear();

  bitboard empty = board_[color][Figure::Empty];
  bitboard opponent = all_[!color];
  bitboard yours = all_[color];
  bitboard all = ~empty;
  bitboard attack = AttackMask(!color);

  BitBoard parent(*this);
  parent.last_pawn_move_ = Position();
  parent.SkipMove();

  bitboard locked_figures = attack & yours & attackFrom(board_[color][Figure::WKing]);

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
  if ((attack & board_[color][Figure::WKing]) != 0)
      index = 0;

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

  if (last_pawn_move_.isValid() && CurrentColor() == color) {
    bitboard sq = 1ULL << last_pawn_move_.index();
    bitboard pawns = board_[color][Figure::WPawn];
    if (color == Color::White) {
        bitboard possible = ((((pawns & from & (~row_a)) >> (8 + 1)) & sq & to)) << (8 + 1);
        if (possible) {
            boards.emplace_back(parent);
            boards.back().Attack(possible, sq << 8, color, Figure::WPawn);
            boards.back().Move(sq << 8, sq, color, Figure::WPawn);
        }
        possible = ((((pawns & from & (~row_h)) >> (8 - 1)) & sq & to)) << (8 - 1);
        if (possible) {
            boards.emplace_back(parent);
            boards.back().Attack(possible, sq << 8, color, Figure::WPawn);
            boards.back().Move(sq << 8, sq, color, Figure::WPawn);
        }
    } else {
        bitboard possible = ((((pawns & from & (~row_h)) << (8 + 1)) & sq & to)) >> (8 + 1);
        if (possible) {
            boards.emplace_back(parent);
            boards.back().Attack(possible, sq >> 8, color, Figure::WPawn);
            boards.back().Move(sq >> 8, sq, color, Figure::WPawn);
        }
        possible = ((((pawns & from & (~row_a)) << (8 - 1)) & sq & to)) >> (8 - 1);
        if (possible) {
            boards.emplace_back(parent);
            boards.back().Attack(possible, sq >> 8, color, Figure::WPawn);
            boards.back().Move(sq >> 8, sq, color, Figure::WPawn);
        }
    }
  }

  auto it =
      std::remove_if(boards.begin() + index, boards.end(),
                     [](const BitBoard &b) { return b.OpponentMateTest(); });
  boards.erase(it, boards.end());

  // rooking
  if (color == Color::White) {
      bitboard right = rooking_flags_.white_oo;
      right = (right << 63) & board_[Color::White][Figure::WRook];
      right >>= 1;
      right &= (empty & ~attack) & to;
      right >>= 1;
      right &= (empty & ~attack);
      right >>= 1;
      right &= (~attack) & board_[Color::White][Figure::WKing] & from;

      bitboard left = rooking_flags_.white_ooo;
      left = (left << 56) & board_[Color::White][Figure::WRook];
      left <<= 1;
      left &= empty;
      left <<= 1;
      left &= (empty & ~attack) & to;
      left <<= 1;
      left &= (empty & ~attack);
      left <<= 1;
      left &= (~attack) & board_[Color::White][Figure::WKing] & from;

      if (right != 0) {
          boards.emplace_back(parent);
          boards.back().Move(1ULL << 60, 1ULL << 62, Color::White, Figure::WKing);
          boards.back().Move(1ULL << 63, 1ULL << 61, Color::White, Figure::WRook);
          boards.back().rooking_flags_.white_oo = false;
          boards.back().rooking_flags_.white_ooo = false;
      }
      if (left != 0) {
          boards.emplace_back(parent);
          boards.back().Move(1ULL << 60, 1ULL << 58, Color::White, Figure::WKing);
          boards.back().Move(1ULL << 56, 1ULL << 59, Color::White, Figure::WRook);
          boards.back().rooking_flags_.white_oo = false;
          boards.back().rooking_flags_.white_ooo = false;
      }
  } else {
      bitboard right = rooking_flags_.black_oo;
      right = (right << 7) & board_[Color::Black][Figure::WRook];
      right >>= 1;
      right &= (empty & ~attack) & to;
      right >>= 1;
      right &= (empty & ~attack);
      right >>= 1;
      right &= (~attack) & board_[Color::Black][Figure::WKing] & from;

      bitboard left = rooking_flags_.black_ooo;
      left = (left << 0) & board_[Color::Black][Figure::WRook];
      left <<= 1;
      left &= empty;
      left <<= 1;
      left &= (empty & ~attack) & to;
      left <<= 1;
      left &= (empty & ~attack);
      left <<= 1;
      left &= (~attack) & board_[Color::Black][Figure::WKing] & from;

      if (right != 0) {
          boards.emplace_back(parent);
          boards.back().Move(1ULL << 4, 1ULL << 6, Color::Black, Figure::WKing);
          boards.back().Move(1ULL << 7, 1ULL << 5, Color::Black, Figure::WRook);
          boards.back().rooking_flags_.black_oo = false;
          boards.back().rooking_flags_.black_ooo = false;
      }
      if (left != 0) {
          boards.emplace_back(parent);
          boards.back().Move(1ULL << 4, 1ULL << 2, Color::Black, Figure::WKing);
          boards.back().Move(1ULL << 0, 1ULL << 3, Color::Black, Figure::WRook);
          boards.back().rooking_flags_.black_oo = false;
          boards.back().rooking_flags_.black_ooo = false;
      }
  }
}

std::vector<BitBoard> BitBoard::generateSubBoards(Color color, bitboard from, bitboard to) const
{
    std::vector<BitBoard> boards;
    boards.reserve(120);

    generateSubBoards(color, boards, from, to);
    return boards;
}

void BitBoard::generateSubBoards(std::vector<BitBoard> &boards, Color color,
                                 uint64_t from, uint64_t to) const {
  generateSubBoards(color, boards, from, to);
}

std::vector<Turn> BitBoard::GenerateTurns(Color color, uint64_t from,
                                          uint64_t to) const {
  std::vector<BitBoard> boards;
  std::vector<Turn> turns;
  turns.reserve(120);
  boards.reserve(120);
  generateSubBoards(boards, color, from, to);

  for (const auto &subboard : boards)
    turns.push_back(GenerateTurn(*this, subboard, color));
  return turns;
}

bool BitBoard::executeTurn(Turn turn) {
  auto maps =
      generateSubBoards(CurrentColor(), operator""_b(turn.from().index()),
                                        operator""_b(turn.to().index()));
  auto turns = BitBoard::GenerateTurns(*this, maps, CurrentColor());
  for (size_t i = 0; i < turns.size(); ++i) {
    if (turns[i] == turn) {
      *this = maps[i];
      return true;
    }
  }
  return false;
}

bool BitBoard::testTurn(Turn turn) const {
  auto turns = GenerateTurns(CurrentColor());
  return std::count(turns.begin(), turns.end(), turn) == 1;
}
*/

template<typename Callback, BitBoard::Flags flags>
class BitBoardHelper
{
    BitBoard board;
    Callback cb;

public:
    using bitboard = BitBoard::bitboard;

    constexpr BitBoardHelper(Callback callback, const BitBoard &board)
        : board(board)
        , cb(callback)
    {}

    template<int delta>
    constexpr Turn generateTurnDelta(bitboard from)
    {
        int pos = log2_64(from);
        return {Position(pos), Position(pos + delta)};
    }
    constexpr Turn generateTurn(bitboard from, bitboard to)
    {
        return {Position(log2_64(from)), Position(log2_64(to))};
    }

    constexpr bitboard getAlies()
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.getWhites();
        else
            return board.getBlacks();
    }
    constexpr bitboard getEnemies()
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.getWhites();
        else
            return board.getBlacks();
    }
    constexpr bitboard &getPawns(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_p;
        else
            return board.m_b_p;
    }
    constexpr bitboard &getEnemyPawns(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) != 0)
            return board.m_w_p;
        else
            return board.m_b_p;
    }
    constexpr bitboard &getKnights(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_n;
        else
            return board.m_b_n;
    }
    constexpr static int getIntReversed(int value)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return value;
        else
            return -value;
    }
    constexpr bitboard &getKing(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_k;
        else
            return board.m_b_k;
    }
    constexpr bitboard &getBishops(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_b;
        else
            return board.m_b_b;
    }
    constexpr bitboard &getRooks(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_r;
        else
            return board.m_b_r;
    }
    constexpr bitboard &getQueens(BitBoard &board)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.m_w_q;
        else
            return board.m_b_q;
    }
    constexpr void removeEnemies(BitBoard &board, bitboard mask)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.removeBlackFigure(mask);
        else
            return board.removeWhiteFigure(mask);
    }
    constexpr void restoreEnemies(BitBoard &board, const BitBoard &from)
    {
        if constexpr ((flags & BitBoard::flags_color) == 0)
            return board.copyBlacks(from);
        else
            return board.copyWhites(from);
    }

    constexpr void generate()
    {
        BitBoard bb(board);
        bb.m_flags = BitBoard::flags_default;
        if constexpr (flags & BitBoard::flags_color)
            bb.m_flags = (BitBoard::Flags)(bb.m_flags | BitBoard::flags_color);
        bb.m_hash = 0;

        bitboard allies = getAlies();
        bitboard enemies = getEnemies();
        bitboard all = allies | enemies;
        bitboard empty = ~all;

        {
            bitboard pawns = getPawns(board);
            bitboard possible, possible_long, possible_left, possible_right;

            if constexpr ((flags & BitBoard::flags_color) == 0) {
                possible = ((pawns & (~line_7)) >> 8) & empty;
                possible_long = ((possible & line_3) >> 8) & empty;
                possible_left = ((pawns & (~line_7)) >> 9) & enemies;
                possible_right = ((pawns & (~line_7)) >> 7) & enemies;
            } else {
                possible = ((pawns & (~line_2)) << 8) & empty;
                possible_long = ((possible & line_6) << 8) & empty;
                possible_left = ((pawns & (~line_2)) << 9) & enemies;
                possible_right = ((pawns & (~line_2)) << 7) & enemies;
            }

            BitForEach(possible, [this, &bb](bitboard bit) {
                bitboard before;
                if constexpr ((flags & BitBoard::flags_color) == 0) {
                    before = bit << 8;
                    bb.m_turn = generateTurnDelta<-8>(before);
                } else {
                    before = bit >> 8;
                    bb.m_turn = generateTurnDelta<8>(before);
                }
                getPawns(bb) = (getPawns(board) & (~before)) | bit;
                cb(bb);
            });

            bb.m_flags = (BitBoard::Flags)(bb.m_flags | BitBoard::flags_el_passant);
            BitForEach(possible_long, [this, &bb](bitboard bit) {
                if constexpr ((flags & BitBoard::flags_color) == 0) {
                    bitboard before = bit << 16;
                    getPawns(bb) = (getPawns(board) & (~before)) | bit;
                    bb.m_turn = generateTurnDelta<-16>(before);
                } else {
                    bitboard before = bit >> 16;
                    getPawns(bb) = (getPawns(board) & (~before)) | bit;
                    bb.m_turn = generateTurnDelta<16>(before);
                }
                cb(bb);
            });
            bb.m_flags = (BitBoard::Flags)(bb.m_flags & (~BitBoard::flags_el_passant));
            getPawns(bb) = getPawns(board);

            bitboard before;
            for (BitIterator iterator(possible_left); iterator.Valid(); ++iterator) {
                if constexpr ((flags & BitBoard::flags_color) == 0) {
                    before = iterator.Bit() << 9;
                    bb.m_turn = generateTurnDelta<-9>(before);
                } else {
                    before = iterator.Bit() >> 9;
                    bb.m_turn = generateTurnDelta<9>(before);
                }
                removeEnemies(bb, ~before);
                getPawns(bb) &= ~before;
                getPawns(bb) |= iterator.Bit();
                cb(bb);
                restoreEnemies(bb, board);
                getPawns(bb) |= before;
                getPawns(bb) &= ~iterator.Bit();
            }

            for (BitIterator iterator(possible_right); iterator.Valid(); ++iterator) {
                if constexpr ((flags & BitBoard::flags_color) == 0) {
                    before = iterator.Bit() << 7;
                    bb.m_turn = generateTurnDelta<-7>(before);
                } else {
                    before = iterator.Bit() >> 7;
                    bb.m_turn = generateTurnDelta<7>(before);
                }
                removeEnemies(bb, ~before);
                getPawns(bb) &= ~before;
                getPawns(bb) |= iterator.Bit();
                cb(bb);
                restoreEnemies(bb, board);
                getPawns(bb) |= before;
                getPawns(bb) &= ~iterator.Bit();
            }

            if constexpr (flags & BitBoard::flags_el_passant) {
                bitboard attack_mask = positionToMask(board.m_turn.to());
                bitboard to_mask = positionToMask(
                    (board.m_turn.from().index() + board.m_turn.to().index()) / 2);
                bitboard left_attack, right_attack;
                if constexpr ((flags & BitBoard::flags_color) == 0) {
                    left_attack = ((pawns >> 9) & to_mask) << 9;
                    right_attack = ((pawns >> 7) & to_mask) << 7;
                } else {
                    left_attack = ((pawns << 9) & to_mask) >> 9;
                    right_attack = ((pawns << 7) & to_mask) >> 7;
                }
                if (left_attack) {
                    getPawns(bb) &= ~left_attack;
                    getPawns(bb) |= to_mask;
                    getEnemyPawns(bb) &= ~attack_mask;
                    constexpr int delta = getIntReversed(-9);
                    bb.m_turn = generateTurnDelta<delta>(left_attack);
                    cb(bb);
                    getPawns(bb) = getPawns(board);
                    getEnemyPawns(bb) = getEnemyPawns(board);
                }
                if (left_attack) {
                    getPawns(bb) &= ~left_attack;
                    getPawns(bb) |= to_mask;
                    getEnemyPawns(bb) &= ~attack_mask;
                    constexpr int delta = getIntReversed(-7);
                    bb.m_turn = generateTurnDelta<delta>(left_attack);
                    cb(bb);
                    getPawns(bb) = getPawns(board);
                    getEnemyPawns(bb) = getEnemyPawns(board);
                }
            }

            if constexpr ((flags & BitBoard::flags_color) == 0) {
                if (bitboard possible = pawns & line_7; possible) {
                    for (BitIterator iterator(possible); iterator.Valid(); ++iterator) {
                        if (bitboard after = (iterator.Bit() >> 8) & empty; after) {
                            bb.m_w_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<-8>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_w_n |= after;
                            cb(bb);
                            bb.m_w_n = board.m_w_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_w_b |= after;
                            cb(bb);
                            bb.m_w_b = board.m_w_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_w_r |= after;
                            cb(bb);
                            bb.m_w_r = board.m_w_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_w_q |= after;
                            cb(bb);
                            bb.m_w_q = board.m_w_q;
                        }
                        if (bitboard after = (iterator.Bit() >> 7) & enemies; after) {
                            bb.m_w_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<-7>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_w_n |= after;
                            cb(bb);
                            bb.m_w_n = board.m_w_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_w_b |= after;
                            cb(bb);
                            bb.m_w_b = board.m_w_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_w_r |= after;
                            cb(bb);
                            bb.m_w_r = board.m_w_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_w_q |= after;
                            cb(bb);
                            bb.m_w_q = board.m_w_q;
                        }
                        if (bitboard after = (iterator.Bit() >> 9) & enemies; after) {
                            bb.m_w_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<-9>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_w_n |= after;
                            cb(bb);
                            bb.m_w_n = board.m_w_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_w_b |= after;
                            cb(bb);
                            bb.m_w_b = board.m_w_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_w_r |= after;
                            cb(bb);
                            bb.m_w_r = board.m_w_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_w_q |= after;
                            cb(bb);
                            bb.m_w_q = board.m_w_q;
                        }
                    }
                }
            } else {
                if (bitboard possible = pawns & line_2; possible) {
                    for (BitIterator iterator(possible); iterator.Valid(); ++iterator) {
                        if (bitboard after = (iterator.Bit() << 8) & empty; after) {
                            bb.m_b_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<8>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_b_n |= after;
                            cb(bb);
                            bb.m_b_n = board.m_b_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_b_b |= after;
                            cb(bb);
                            bb.m_b_b = board.m_b_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_b_r |= after;
                            cb(bb);
                            bb.m_b_r = board.m_b_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_b_q |= after;
                            cb(bb);
                            bb.m_b_q = board.m_b_q;
                        }
                        if (bitboard after = (iterator.Bit() << 7) & enemies; after) {
                            bb.m_b_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<7>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_b_n |= after;
                            cb(bb);
                            bb.m_b_n = board.m_b_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_b_b |= after;
                            cb(bb);
                            bb.m_b_b = board.m_b_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_b_r |= after;
                            cb(bb);
                            bb.m_b_r = board.m_b_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_b_q |= after;
                            cb(bb);
                            bb.m_b_q = board.m_b_q;
                        }
                        if (bitboard after = (iterator.Bit() << 9) & enemies; after) {
                            bb.m_b_p &= ~iterator.Bit();
                            bb.m_turn = generateTurnDelta<9>(iterator.Bit());
                            bb.m_turn.setFigure(Figure::Knight);
                            bb.m_b_n |= after;
                            cb(bb);
                            bb.m_b_n = board.m_b_n;
                            bb.m_turn.setFigure(Figure::Bishop);
                            bb.m_b_b |= after;
                            cb(bb);
                            bb.m_b_b = board.m_b_b;
                            bb.m_turn.setFigure(Figure::Rook);
                            bb.m_b_r |= after;
                            cb(bb);
                            bb.m_b_r = board.m_b_r;
                            bb.m_turn.setFigure(Figure::Queen);
                            bb.m_b_q |= after;
                            cb(bb);
                            bb.m_b_q = board.m_b_q;
                        }
                    }
                }
            }
            getPawns(bb) = getPawns(board);
        }

        {
            //knight constexpr generated moves
            bitboard knights = getKnights(board);
            for (BitIterator iterator(knights); iterator.Valid(); ++iterator) {
                bitboard bit = iterator.Bit();
                Position from = log2_64(bit);
                bitboard possible = processKnight(from);
                bitboard moves = possible & empty;
                bitboard attacks = possible & enemies;

                getKnights(bb) &= ~bit;

                for (BitIterator it2(moves); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getKnights(bb) |= bit2;
                    bb.m_turn = Turn(from, to, false);
                    cb(bb);
                    getKnights(bb) &= ~bit2;
                }

                for (BitIterator it2(attacks); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getKnights(bb) |= bit2;
                    bb.m_turn = Turn(from, to, true);
                    removeEnemies(bb, ~bit2);
                    cb(bb);
                    restoreEnemies(bb, board);
                    getKnights(bb) &= ~bit2;
                }

                getKnights(bb) |= bit;
            }
        }
        //king constexpr generated moves
        if (bitboard king = getKing(board); king) {
            Position from = log2_64(king);
            bitboard possible = processKing(from);
            bitboard moves = possible & empty;
            bitboard attacks = possible & enemies;

            getKing(bb) &= ~king;

            for (BitIterator it2(moves); it2.Valid(); ++it2) {
                Position to = log2_64(it2.Bit());
                getKing(bb) |= it2.Bit();
                bb.m_turn = Turn(from, to, false);
                cb(bb);
                getKing(bb) &= ~it2.Bit();
            }

            for (BitIterator it2(attacks); it2.Valid(); ++it2) {
                Position to = log2_64(it2.Bit());
                getKing(bb) |= it2.Bit();
                bb.m_turn = Turn(from, to, true);
                removeEnemies(bb, ~it2.Bit());
                cb(bb);
                restoreEnemies(bb, board);
                getKing(bb) &= ~it2.Bit();
            }

            getKing(bb) |= king;
        }
        {
            //bishops with magic bitboards
            bitboard bishops = getBishops(board);
            for (BitIterator iterator(bishops); iterator.Valid(); ++iterator) {
                bitboard bit = iterator.Bit();
                Position from = log2_64(bit);
                bitboard possible = processBishop(from, all);
                bitboard moves = possible & empty;
                bitboard attacks = possible & enemies;

                getBishops(bb) &= ~bit;

                for (BitIterator it2(moves); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getBishops(bb) |= bit2;
                    bb.m_turn = Turn(from, to, false);
                    cb(bb);
                    getBishops(bb) &= ~bit2;
                }

                for (BitIterator it2(attacks); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getBishops(bb) |= bit2;
                    bb.m_turn = Turn(from, to, true);
                    removeEnemies(bb, ~bit2);
                    cb(bb);
                    restoreEnemies(bb, board);
                    getBishops(bb) &= ~bit2;
                }

                getBishops(bb) |= bit;
            }
        }
        {
            //rooks with magic bitboards
            bitboard rooks = getBishops(board);
            for (BitIterator iterator(rooks); iterator.Valid(); ++iterator) {
                bitboard bit = iterator.Bit();
                Position from = log2_64(bit);
                bitboard possible = processRook(from, all);
                bitboard moves = possible & empty;
                bitboard attacks = possible & enemies;

                getRooks(bb) &= ~bit;

                for (BitIterator it2(moves); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getRooks(bb) |= bit2;
                    bb.m_turn = Turn(from, to, false);
                    cb(bb);
                    getRooks(bb) &= ~bit2;
                }

                for (BitIterator it2(attacks); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getRooks(bb) |= bit2;
                    bb.m_turn = Turn(from, to, true);
                    removeEnemies(bb, ~bit2);
                    cb(bb);
                    restoreEnemies(bb, board);
                    getRooks(bb) &= ~bit2;
                }

                getRooks(bb) |= bit;
            }
        }
        {
            //rooks with magic bitboards
            bitboard queens = getQueens(board);
            for (BitIterator iterator(queens); iterator.Valid(); ++iterator) {
                bitboard bit = iterator.Bit();
                Position from = log2_64(bit);
                bitboard possible = processBishop(from, all) | processRook(from, all);
                bitboard moves = possible & empty;
                bitboard attacks = possible & enemies;

                getQueens(bb) &= ~bit;

                for (BitIterator it2(moves); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getQueens(bb) |= bit2;
                    bb.m_turn = Turn(from, to, false);
                    cb(bb);
                    getQueens(bb) &= ~bit2;
                }

                for (BitIterator it2(attacks); it2.Valid(); ++it2) {
                    bitboard bit2 = it2.Bit();
                    Position to = log2_64(bit2);
                    getQueens(bb) |= bit2;
                    bb.m_turn = Turn(from, to, true);
                    removeEnemies(bb, ~bit2);
                    cb(bb);
                    restoreEnemies(bb, board);
                    getQueens(bb) &= ~bit2;
                }

                getQueens(bb) |= bit;
            }
        }
    }
};

BitBoard BitBoard::set(Position position, Figure figure) const
{
    BitBoard copy(*this);
    bitboard mask = positionToMask(position);

    switch (figure) {
    case Figure::Empty:
        copy.removeFigure(~mask);
        break;
    case Figure::WPawn:
        copy.removeFigure(~mask);
        copy.m_w_p |= mask;
        break;
    case Figure::WKnight:
        copy.removeFigure(~mask);
        copy.m_w_n |= mask;
        break;
    case Figure::WBishop:
        copy.removeFigure(~mask);
        copy.m_w_b |= mask;
        break;
    case Figure::WRook:
        copy.removeFigure(~mask);
        copy.m_w_r |= mask;
        break;
    case Figure::WQueen:
        copy.removeFigure(~mask);
        copy.m_w_q |= mask;
        break;
    case Figure::WKing:
        copy.removeFigure(~mask);
        copy.m_w_k |= mask;
        break;
    case Figure::BPawn:
        copy.removeFigure(~mask);
        copy.m_b_p |= mask;
        break;
    case Figure::BKnight:
        copy.removeFigure(~mask);
        copy.m_b_n |= mask;
        break;
    case Figure::BBishop:
        copy.removeFigure(~mask);
        copy.m_b_b |= mask;
        break;
    case Figure::BRook:
        copy.removeFigure(~mask);
        copy.m_b_r |= mask;
        break;
    case Figure::BQueen:
        copy.removeFigure(~mask);
        copy.m_b_q |= mask;
        break;
    case Figure::BKing:
        copy.removeFigure(~mask);
        copy.m_b_k |= mask;
        break;
    }

    return copy;
}

BitBoard BitBoard::setFlags(Flags flags) const
{
    BitBoard board(*this);
    board.m_flags = flags;
    return board;
}

BitBoard BitBoard::setTurn(Turn turn) const
{
    BitBoard board(*this);
    board.m_turn = turn;
    return board;
}

BitBoard BitBoard::swap(Position p1, Position p2) const
{
    if (p1 == p2)
        return *this;
    auto figure1 = get(p1);
    auto figure2 = get(p2);
    return set(p2, figure1).set(p1, figure2);
}

Figure BitBoard::get(Position position) const noexcept
{
    bitboard mask = positionToMask(position);
    if (mask & m_w_p)
        return Figure::WPawn;
    if (mask & m_w_n)
        return Figure::WKnight;
    if (mask & m_w_b)
        return Figure::WBishop;
    if (mask & m_w_r)
        return Figure::WRook;
    if (mask & m_w_q)
        return Figure::WQueen;
    if (mask & m_w_k)
        return Figure::WKing;
    if (mask & m_b_p)
        return Figure::BPawn;
    if (mask & m_b_n)
        return Figure::BKnight;
    if (mask & m_b_b)
        return Figure::BBishop;
    if (mask & m_b_r)
        return Figure::BRook;
    if (mask & m_b_q)
        return Figure::BQueen;
    if (mask & m_b_k)
        return Figure::BKing;
    return Figure::Empty;
}

BitBoard::Flags BitBoard::getFlags() const noexcept
{
    return m_flags;
}

Color BitBoard::getCurrentSide() const noexcept
{
    return m_flags & Flags::flags_color ? Color::Black : Color::White;
}

/* 
inline void BitBoard::processPawns(const BitBoard &parrent,
                                   std::vector<BitBoard> &boards,
                                   Color color,
                                   bitboard pawns,
                                   bitboard empty,
                                   bitboard allies,
                                   bitboard opponents)
{

    if (color == Color::White)
        possible = (((map & (~row_a)) >> (8 + 1)) & opponent & to_mask) << (8 + 1);
    else
        possible = (((map & (~row_h)) << (8 + 1)) & opponent & to_mask) >> (8 + 1);
    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        if (color == Color::White)
            after = iterator.Bit() >> (8 + 1);
        else
            after = iterator.Bit() << (8 + 1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(), after, color, Figure::WPawn);
        boards.emplace_back(copy);
        if (after & (line_1 | line_8)) {
            boards.back().Transform(after, color, Figure::WPawn, Figure::WQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WRook);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WKnight);
        }

        ++iterator;
    }

    if (color == Color::White)
        possible = (((map & (~row_h)) >> (8 - 1)) & opponent & to_mask) << (8 - 1);
    else
        possible = (((map & (~row_a)) << (8 - 1)) & opponent & to_mask) >> (8 - 1);

    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        if (color == Color::White)
            after = iterator.Bit() >> (8 - 1);
        else
            after = iterator.Bit() << (8 - 1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(), after, color, Figure::WPawn);
        boards.emplace_back(copy);
        if (after & (line_1 | line_8)) {
            boards.back().Transform(after, color, Figure::WPawn, Figure::WQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WRook);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after, color, Figure::WPawn, Figure::WKnight);
        }

        ++iterator;
    }

    if (color == Color::White)
        possible = (((((map & line_2) >> 8) & ~all) >> 8) & ~all & to_mask) << 2 * 8;
    else
        possible = (((((map & line_7) << 8) & ~all) << 8) & ~all & to_mask) >> 2 * 8;
    iterator = possible;

    while (iterator.Valid()) {
        bitboard after;
        bitboard el_passant;
        if (color == Color::White) {
            after = iterator.Bit() >> 2 * 8;
            el_passant = iterator.Bit() >> 8;
        } else {
            after = iterator.Bit() << 2 * 8;
            el_passant = iterator.Bit() << 8;
        }

        boards.emplace_back(parrent);
        boards.back().last_pawn_move_ = log2_64(el_passant);
        boards.back().Move(iterator.Bit(), after, color, Figure::WPawn);

        ++iterator;
    }
}
*/

template<typename Callback, BitBoard::Flags flags>
constexpr void generateTemplate(Callback callback, const BitBoard &board)
{
    BitBoardHelper<Callback, flags>(callback, board).generate();
}

template<typename Callback, std::size_t... I>
constexpr auto generatePointerArray(std::index_sequence<I...>)
{
    using pointer = void (*)(Callback, const BitBoard &);
    constexpr std::size_t N = sizeof...(I);
    return std::array<pointer, N>{{&generateTemplate<Callback, (BitBoard::Flags) I>...}};
}

template<typename Callback>
constexpr void generate(Callback callback, const BitBoard &board, BitBoard::Flags flags)
{
    constexpr static auto pointers = generatePointerArray<Callback>(
        std::make_index_sequence<BitBoard::flags_upper_bound>{});
    pointers[flags](callback, board);
}

std::vector<BitBoard> BitBoard::generateSubBoards(Color color, bitboard from, bitboard to) const
{
    std::vector<BitBoard> boards;
    boards.reserve(120);
    auto flags = m_flags;
    if (color == Color::White)
        flags = (Flags) (flags & (~flags_color));
    else
        flags = (Flags) (flags | flags_color);

    generate([&](const BitBoard &board) { boards.emplace_back(board); }, *this, flags);
    return boards;
}

int BitBoard::getSubBoardsCounter(Color color, bitboard from, bitboard to) const
{
    auto flags = m_flags;
    if (color == Color::White)
        flags = (Flags) (flags & (~flags_color));
    else
        flags = (Flags) (flags | flags_color);

    int count;
    generate([&](const BitBoard &board) { count++; }, *this, flags);
    return count;
}

BitBoard BitBoard::executeTurn(Turn turn)
{
    return {};
}

bool BitBoard::testTurn(Turn turn) const
{
    return {};
}

BitBoard::BitBoardHash BitBoard::getHash() const
{
    return m_hash;
}

Turn BitBoard::getTurn() const
{
    return m_turn;
}
