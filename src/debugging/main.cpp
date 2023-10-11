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

  ab.SetBoard(BitBoard());

  ab.Start();
  std::this_thread::sleep_for(std::chrono::milliseconds{500});
  ab.Stop();
  auto pv = ab.GetPV();
  for (auto eleme : pv) std::cout << eleme.ToChessFormat() << " ";
}
