#include "evaluate.hpp"

#include <magic.hpp>

// material
constexpr static int material[7] = {0, 126, 781, 825, 1276, 2538, 0};
constexpr static int pawn = 126;
constexpr static int knight = 781;
constexpr static int bishop = 825;
constexpr static int rook = 1276;
constexpr static int queen = 2538;

// moves counter
constexpr static int movementbonus = 3;
constexpr static int figure_safety_bonus = 12;

// position bonuses

constexpr static int pawn_bonus[64]{
    0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
    10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
    0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
    5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0};
constexpr static int knight_bonus[64]{
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   0,   0,
    0,   -20, -40, -30, 0,   10,  15,  15,  10,  0,   -30, -30, 5,
    15,  20,  20,  15,  5,   -30, -30, 0,   15,  20,  20,  15,  0,
    -30, -30, 5,   10,  15,  15,  10,  5,   -30, -40, -20, 0,   5,
    5,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50,
};
constexpr static int bishop_bonus[64]{
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 0,   0,   0,   0,
    0,   0,   -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 5,
    5,   10,  10,  5,   5,   -10, -10, 0,   10,  10,  10,  10,  0,
    -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 5,   0,   0,
    0,   0,   5,   -10, -20, -10, -10, -10, -10, -10, -10, -20,
};
constexpr static int rook_bonus[64]{
    0,  0, 0, 0, 0, 0, 0, 0,  5,  10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, -5, 0,  0,  0,  0,  0,  0,  -5,
    -5, 0, 0, 0, 0, 0, 0, -5, 0,  0,  0,  5,  5,  0,  0,  0};
constexpr static int queen_bonus[64]{
    -20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   0,   0,  0,  0,   0,   -10,
    -10, 0,   5,   5,  5,  5,   0,   -10, -5,  0,   5,   5,  5,  5,   0,   -5,
    0,   0,   5,   5,  5,  5,   0,   -5,  -10, 5,   5,   5,  5,  5,   0,   -10,
    -10, 0,   5,   0,  0,  0,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20};
constexpr static int king_bonus[64]{
    -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
    -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
    -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
    -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
    0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20};

constexpr static int doubled_pawn_punishment = 100;
constexpr static int doubled_kb_bonus = 176;

// rangement
constexpr static int invalid = std::numeric_limits<int>::min();
constexpr static int prange = std::numeric_limits<int>::max();
constexpr static int nrange = -prange;

int CalculateBonus(bitboard_t map, const int bonus[], Color color) {
  int value = 0;
  const int scale = 3;
  BitIterator i(map);
  for (; i.Valid(); ++i) {
    Position index = log2_64(i.Bit());
    value += scale *
             bonus[(color == Color::kWhite ? index : index.Rotate()).Value()];
  }
  return value;
}

Score::Score() : value_(invalid) {}

Score::Score(long int value) {
  if (value > prange)
    value_ = prange;
  else if (value < nrange)
    value_ = nrange;
  else
    value_ = value;
}

bool Score::operator<(Score value) const { return value_ < value.value_; }
bool Score::operator>(Score value) const { return value_ > value.value_; }
bool Score::operator<=(Score value) const { return value_ <= value.value_; }
bool Score::operator>=(Score value) const { return value_ >= value.value_; }
bool Score::operator==(Score value) const { return value_ == value.value_; }
bool Score::operator!=(Score value) const { return value_ != value.value_; }

Score Score::operator-(Score value) const {
  if (value_ == invalid) return Score{invalid};
  if (value.value_ == invalid) return Score{invalid};

  long long int val = (long long int)value_ - value.value_;

  if (val < nrange)
    return Score{nrange};
  else if (val > prange)
    return Score{prange};
  return Score(val);
}

Score Score::operator+(const Score value) const {
  if (value_ == invalid) return Score{invalid};
  if (value.value_ == invalid) return Score{invalid};

  long long int val = (long long int)value_ + value.value_;

  if (val < nrange)
    return Score{nrange};
  else if (val > prange)
    return Score{prange};
  return Score(val);
}

Score Score::operator-() const {
  if (value_ == invalid) return Score{invalid};

  return Score(-value_);
}

Score Score::Win(int depth) { return Score(prange - depth - 1); }

Score Score::Lose(int depth) { return Score(nrange + depth + 1); }

Score Score::Tie() { return Score(0); }

Score Score::Value(const BitBoard &board, Color color) {
  int value = 0;

  // pawns
  value += pawn * count_1s(board.GetBitBoard(Color::kWhite, Figure::kPawn));
  value -= pawn * count_1s(board.GetBitBoard(Color::kBlack, Figure::kPawn));

  {
    auto wmap = board.AttackMask(Color::kWhite) &
                ~board.GetColorBitBoard(Color::kWhite);
    value += movementbonus * count_1s(wmap);

    auto bmap = board.AttackMask(Color::kBlack) &
                ~board.GetColorBitBoard(Color::kBlack);
    value -= movementbonus * count_1s(bmap);
  }
  {
    auto wmap =
        board.AttackMask(Color::kWhite) & board.GetColorBitBoard(Color::kWhite);
    value += figure_safety_bonus * count_1s(wmap);

    auto bmap =
        board.AttackMask(Color::kBlack) & board.GetColorBitBoard(Color::kBlack);
    value -= figure_safety_bonus * count_1s(bmap);
  }

  for (int i = 0; i < 8; ++i) {
    auto white_pawns =
        board.GetBitBoard(Color::kWhite, Figure::kPawn) & rows[i];
    auto black_pawns =
        board.GetBitBoard(Color::kBlack, Figure::kPawn) & rows[i];
    if (count_1s(white_pawns) > 1) value -= doubled_pawn_punishment;
    if (count_1s(black_pawns) > 1) value += doubled_pawn_punishment;
  }

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kPawn),
                          pawn_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kPawn),
                          pawn_bonus, Color::kBlack);

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kKnight),
                          knight_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kKnight),
                          knight_bonus, Color::kBlack);

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kBishop),
                          bishop_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kBishop),
                          bishop_bonus, Color::kBlack);

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kRook),
                          rook_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kRook),
                          rook_bonus, Color::kBlack);

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kQueen),
                          queen_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kQueen),
                          queen_bonus, Color::kBlack);

  value += CalculateBonus(board.GetBitBoard(Color::kWhite, Figure::kKing),
                          king_bonus, Color::kWhite);
  value -= CalculateBonus(board.GetBitBoard(Color::kBlack, Figure::kKing),
                          king_bonus, Color::kBlack);

  if (count_1s(board.GetBitBoard(Color::kWhite, Figure::kKnight)) == 2)
    value += doubled_kb_bonus;
  if (count_1s(board.GetBitBoard(Color::kBlack, Figure::kKnight)) == 2)
    value -= doubled_kb_bonus;
  if (count_1s(board.GetBitBoard(Color::kWhite, Figure::kBishop)) == 2)
    value += doubled_kb_bonus;
  if (count_1s(board.GetBitBoard(Color::kBlack, Figure::kBishop)) == 2)
    value -= doubled_kb_bonus;

  value += knight * count_1s(board.GetBitBoard(Color::kWhite, Figure::kKnight));
  value -= knight * count_1s(board.GetBitBoard(Color::kBlack, Figure::kKnight));
  value += bishop * count_1s(board.GetBitBoard(Color::kWhite, Figure::kBishop));
  value -= bishop * count_1s(board.GetBitBoard(Color::kBlack, Figure::kBishop));
  value += rook * count_1s(board.GetBitBoard(Color::kWhite, Figure::kRook));
  value -= rook * count_1s(board.GetBitBoard(Color::kBlack, Figure::kRook));
  value += queen * count_1s(board.GetBitBoard(Color::kWhite, Figure::kQueen));
  value -= queen * count_1s(board.GetBitBoard(Color::kBlack, Figure::kQueen));

  if (color == Color::kBlack) value = -value;

  return Score{value};
}

Score Score::Max() { return Score(prange); }

Score Score::Min() { return Score(nrange); }

Score::ScoreType Score::FigurePrice(Figure figure) { return material[figure]; }

Score::ScoreType Score::Value() const { return value_; }

std::string Score::ToString() const { return std::to_string(value_); }

float Score::ToCentiPawns() const { return (float)value_ / pawn / 28 * 10; }
