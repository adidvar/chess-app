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
      BitBoard{"7k/1p2N3/p4p2/1r1p2p1/8/P7/5KPP/5RR1 w - - 0 0"});
  return 0;
}
