#include "zobrist.hpp"

#include <random>

bitboard_t FiguresHash[Figure::Max()][Color::Max()][Position::Max()];
bitboard_t ShortCastlingFlagHash[Color::Max()];
bitboard_t LongCastlingFlagHash[Color::Max()];
bitboard_t CurrentColorHash[Color::Max()];
bitboard_t ElpassantHash[Position::Max()];

uint64_t GetFigureHash(Figure figure, Color color, Position position) {
  return FiguresHash[figure][color][position.Value()];
}

uint64_t GetShortCastlingFlagHash(Color color) {
  return ShortCastlingFlagHash[color];
}

uint64_t GetLongCastlingFlagHash(Color color) {
  return LongCastlingFlagHash[color];
}

uint64_t GetTurnCounterHash(int count) { return 0; }

uint64_t GetPassiveCountHash(int count) { return 0; }

uint64_t GetCurrentColorHash(Color color) { return CurrentColorHash[color]; }

uint64_t GetElpassantHash(Position position) {
  return ElpassantHash[position.Value()];
}

void InitZobrist() {
  std::mt19937_64 random(0ULL);

  for (size_t f = 0; f < Figure::Max(); ++f)
    for (size_t c = 0; c < Color::Max(); ++c)
      for (size_t p = 0; p < Position::Max(); ++p)
        FiguresHash[f][c][p] = random();

  for (size_t c = 0; c < Color::Max(); ++c) ShortCastlingFlagHash[c] = random();
  for (size_t c = 0; c < Color::Max(); ++c) LongCastlingFlagHash[c] = random();
  for (size_t c = 0; c < Color::Max(); ++c) CurrentColorHash[c] = random();
  for (size_t p = 0; p < Position::Max(); ++p) ElpassantHash[p] = random();
}

struct StartUp {
  StartUp() { InitZobrist(); }
} static startup;
