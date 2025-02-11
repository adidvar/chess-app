#include <chessai/computer.hpp>
#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>
#include <iostream>

int main()
{
  Turn turns[216];
  BitBoard board("4k3/Q7/8/8/8/8/8/2K4Q w - - 0 0");
  bool in_mate;
  auto count = board.getTurns(board.getCurrentSide(), turns, in_mate);

  for (int i = 0; i < count; i++) {
    std::cout << turns[i].toString() << std::endl;
  }

  return 0;
}
