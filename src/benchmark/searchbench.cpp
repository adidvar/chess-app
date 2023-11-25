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
  /*
  for (auto pair : stockfish_result) {
    TTable table;
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);

    float value = cmp.GetValue(BitBoard{pair.first}, 6).ToCentiPawns();
    auto stat = cmp.GetStatistics();
    float error = value - pair.second;

    std::cout << pair.first << ":::::" << value << "::::" << pair.second
              << std::endl;

    delta += error * error;
  }
  std::cout << "Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;

  std::cout << std::endl;
  std::cout << "Delta with stockfish: " << sqrt(delta) << std::endl;


  for (auto pair : stockfish_result) {
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    TTable table;
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    cmp.GetValue(BitBoard(pair.first), 6);
    statistics += cmp.GetStatistics();
    std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  }
  std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  statistics.Clear();
*/

  begin = std::chrono::high_resolution_clock::now();

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
