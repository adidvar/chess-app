#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iomanip>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>

#include "pvs.hpp"

int main() {
  float delta = 0;

  auto begin = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  Statistics statistics;

  AspirationWindow window;

  for (int d = 1; d <= 11; d++) {
    TTable table;
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    std::cout << "Forecast: " << window.GetLow().ToCentiPawns() << " <--> "
              << window.GetHigh().ToCentiPawns() << std::endl;

    begin = std::chrono::high_resolution_clock::now();
    std::string fen{"startpos"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    window.Push(value);
    statistics = cmp.GetStatistics();
    // auto turn = cmp.GetTurn(BitBoard{fen}, d);

    auto end = std::chrono::high_resolution_clock::now();
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
            .count();

    std::cout << std::setw(8) << d << ": " << std::setw(8)
              << statistics.GetMainNode() << "  :  " << std::setw(10)
              << value.ToCentiPawns() << " : " << std::setw(10) << msec << " : "
              << std::endl;
    //  << turn.ToChessFormat() << std::endl;
  }
}
