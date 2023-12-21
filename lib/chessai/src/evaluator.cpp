#include "evaluator.hpp"

#include "magic.hpp"

Evaluator::Evaluator(const BitBoard &board, Color color,
                     const GameStage &settings)
    : m_board(board),
      m_color(color),
      m_settings(settings),
      white_mask(board.AttackMask(Color::kWhite)),
      black_mask(board.AttackMask(Color::kBlack)) {}

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

Score::ProcessType Evaluator::Evaluate() {
  int value = 0;

  // pawns
  value += pawn * count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kPawn));
  value -= pawn * count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kPawn));

  {
    auto wmap = m_board.AttackMask(Color::kWhite) &
                ~m_board.GetColorBitBoard(Color::kWhite);
    value += movementbonus * count_1s(wmap);

    auto bmap = m_board.AttackMask(Color::kBlack) &
                ~m_board.GetColorBitBoard(Color::kBlack);
    value -= movementbonus * count_1s(bmap);
  }
  {
    auto wmap = m_board.AttackMask(Color::kWhite) &
                m_board.GetColorBitBoard(Color::kWhite);
    value += figure_safety_bonus * count_1s(wmap);

    auto bmap = m_board.AttackMask(Color::kBlack) &
                m_board.GetColorBitBoard(Color::kBlack);
    value -= figure_safety_bonus * count_1s(bmap);
  }

  for (int i = 0; i < 8; ++i) {
    auto white_pawns =
        m_board.GetBitBoard(Color::kWhite, Figure::kPawn) & rows[i];
    auto black_pawns =
        m_board.GetBitBoard(Color::kBlack, Figure::kPawn) & rows[i];
    if (count_1s(white_pawns) > 1) value -= doubled_pawn_punishment;
    if (count_1s(black_pawns) > 1) value += doubled_pawn_punishment;
  }

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kPawn),
                          pawn_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kPawn),
                          pawn_bonus, Color::kBlack);

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kKnight),
                          knight_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kKnight),
                          knight_bonus, Color::kBlack);

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kBishop),
                          bishop_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kBishop),
                          bishop_bonus, Color::kBlack);

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kRook),
                          rook_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kRook),
                          rook_bonus, Color::kBlack);

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kQueen),
                          queen_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kQueen),
                          queen_bonus, Color::kBlack);

  value += CalculateBonus(m_board.GetBitBoard(Color::kWhite, Figure::kKing),
                          king_bonus, Color::kWhite);
  value -= CalculateBonus(m_board.GetBitBoard(Color::kBlack, Figure::kKing),
                          king_bonus, Color::kBlack);

  if (count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kKnight)) == 2)
    value += doubled_kb_bonus;
  if (count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kKnight)) == 2)
    value -= doubled_kb_bonus;
  if (count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kBishop)) == 2)
    value += doubled_kb_bonus;
  if (count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kBishop)) == 2)
    value -= doubled_kb_bonus;

  value +=
      knight * count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kKnight));
  value -=
      knight * count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kKnight));
  value +=
      bishop * count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kBishop));
  value -=
      bishop * count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kBishop));
  value += rook * count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kRook));
  value -= rook * count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kRook));
  value += queen * count_1s(m_board.GetBitBoard(Color::kWhite, Figure::kQueen));
  value -= queen * count_1s(m_board.GetBitBoard(Color::kBlack, Figure::kQueen));

  if (m_color == Color::kBlack) value = -value;

  return value;
}

Score::ProcessType Evaluator::GetFigurePrice(Figure figure) {
  return material[figure];
}
