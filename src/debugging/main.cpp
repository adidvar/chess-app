#include <alphabeta.hpp>
#include <computer.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

int main() {
  /*
  std::atomic_bool flag = 0;
  ItDeepening<Evaluate> ab(Color::kWhite, flag);
  TTable table;
  ab.SetTTable(&table);
  std::thread thread([&flag]() {
    std::this_thread::sleep_for(std::chrono::milliseconds{10000});
    flag = 1;
  });
  auto pv = ab.FindPV(BitBoard(), 10);
  for (auto eleme : pv) std::cout << eleme.ToChessFormat() << " ";
  thread.join();
*/

  Computer ab(Color::kWhite);

  ab.SetBoard(BitBoard(
      "4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32"));

  ab.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{10000});
  ab.Stop();
  auto pv = ab.GetPV();
  for (auto eleme : pv) std::cout << eleme.ToChessFormat() << " ";
}
