#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iomanip>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>


int main() {
  float delta = 0;

  /*
  auto lambda = []() {
    auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    Statistics statistics;

    for (int d = 1; d <= 8; d++) {
      TTable table;
      ItDeepening cmp({"startpos"}, Color::kWhite);
      cmp.SetTTable(&table);
      cmp.SetStopFlag(nullptr);

      begin = std::chrono::high_resolution_clock::now();
      auto value = cmp.GetValue(d);
      auto pv = cmp.FindPV();
      statistics = cmp.GetStatistics();

      auto end = std::chrono::high_resolution_clock::now();
      auto msec =
          std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
              .count();

      std::cout << std::setw(8) << d << ": " << std::setw(8)
                << statistics.GetMainNode() << "  :  " << std::setw(10) << " : "
                << value.ToCentiPawns() << std::setw(10) << msec << " : ";

      for (auto elem : pv) std::cout << elem.ToChessFormat() << " ";
      std::cout << std::endl;
      //  << turn.ToChessFormat() << std::endl;
    }
  };

  lambda();
*/

  Computer computer;
  computer.SetBoard({"startpos"});
  computer.Start();
  std::this_thread::sleep_for(std::chrono::seconds{6});
  computer.Abort();
  std::cout << computer.Get().ToChessFormat() << std::endl;
  std::cout << "end of code" << std::endl;
}
