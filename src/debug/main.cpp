#include <chesscore/bitboard.hpp>
#include <computer.hpp>
#include <score.hpp>

int main()
{
  Computer computer;
  computer.SetBoard(BitBoard{"startpos"});
  computer.Start();
  computer.Abort();

  return 0;
}
