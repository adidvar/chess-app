#include <alphabeta.hpp>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

int main() {
  std::atomic_bool flag = 0;
  ItDeepening<Evaluate> ab(Color::kWhite, flag);
  std::thread thread([&flag]() {
    std::this_thread::sleep_for(std::chrono::milliseconds{2000});
    flag = 1;
  });
  auto pv = ab.FindPV(BitBoard());
  for (auto eleme : pv) std::cout << eleme.ToChessFormat() << " ";
  thread.join();
}
