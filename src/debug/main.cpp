#include <chessai/computer.hpp>
#include <chessai/score.hpp>
#include <chesscore/bitboard.hpp>

int main()
{
  Computer computer;
  computer.SetBoard(BitBoard{"startpos"});
  computer.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{2000});
  std::cout << "end" << std::endl;
  computer.Abort();
  std::cout << "end" << std::endl;

  return 0;
}
