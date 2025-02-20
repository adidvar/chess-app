#include <chessai/computer.hpp>
#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>
#include <iostream>

size_t getTurns(BitBoard board) {
  Turn turns[216];
  bool in_mate;

  auto count = board.getTurns(board.side(), turns, in_mate);

  for (int i = 0; i < count; i++) std::cout << turns[i].toString() << std::endl;
  return count;
}

int main()
{
  std::cout << getTurns(
      BitBoard{"rnbqkbnr/pppppp1p/8/8/7P/3P3p/PPP1PPP1/RNBQKBN1 w Qkq - 0 1"});
  std::cout
      << BitBoard{"rnbqkbnr/pppppp1p/8/8/6pP/3P3R/PPP1PPP1/RNBQKBN1 w Qkq - 0 1"}
             .executeTurn(Color::White, Turn("g4h3"))
             .fen();
  return 0;
}
