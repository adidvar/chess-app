#include "searchbench.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

#include "alphabeta.hpp"
#include "itdeepening.hpp"
#include "pvs.hpp"
#include "statistics.hpp"

void SearchBenchmark() {
  float delta = 0;

  auto begin = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  Statistics statistics;

  TTable table;
  for (int d = 1; d <= 8; d++) {
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    std::string fen{"startpos"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    statistics += cmp.GetStatistics();
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

  begin = std::chrono::high_resolution_clock::now();
  table.Clear();
  statistics.Clear();
  for (int d = 1; d <= 8; d++) {
    ItDeepening<PVS> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    std::string fen{"startpos"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    statistics += cmp.GetStatistics();
    // auto turn = cmp.GetTurn(BitBoard{fen}, d);

    auto end = std::chrono::high_resolution_clock::now();
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
            .count();

    std::cout << std::setw(8) << d << ": " << std::setw(8)
              << statistics.GetMainNode() << "  :  " << std::setw(10)
              << value.ToCentiPawns() << " : " << std::setw(10) << msec << " : "
              << std::endl;
    //<< turn.ToChessFormat() << std::endl;
  }

  begin = std::chrono::high_resolution_clock::now();
  table.Clear();
  statistics.Clear();
  for (int d = 1; d <= 8; d++) {
    PVS cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    std::string fen{"startpos"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    statistics += cmp.GetStatistics();
    // auto turn = cmp.GetTurn(BitBoard{fen}, d);

    auto end = std::chrono::high_resolution_clock::now();
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
            .count();

    std::cout << std::setw(8) << d << ": " << std::setw(8)
              << statistics.GetMainNode() << "  :  " << std::setw(10)
              << value.ToCentiPawns() << " : " << std::setw(10) << msec << " : "
              << std::endl;
    //<< turn.ToChessFormat() << std::endl;
  }
}
