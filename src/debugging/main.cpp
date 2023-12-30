#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iomanip>
#include <iostream>
#include <itdeepening.hpp>
#include <minmax.hpp>


int main() {
  float delta = 0;

  auto begin = std::chrono::high_resolution_clock::now();
  auto end = std::chrono::high_resolution_clock::now();
  Statistics statistics;

  for (int d = 0; d <= 11; d++) {
    TTable table;
    ItDeepening<AlphaBeta> cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    begin = std::chrono::high_resolution_clock::now();
    std::string fen{
        "rn5r/p2q1p1k/bp2N2p/3pP3/Pb3Q2/1P3NP1/5PB1/R3R1K1 w - - 1 23"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    statistics = cmp.GetStatistics();
    auto turn = cmp.GetTurn(BitBoard{fen});

    auto end = std::chrono::high_resolution_clock::now();
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
            .count();

    std::cout << std::setw(8) << d << ": " << std::setw(8)
              << statistics.GetMainNode() << "  :  " << std::setw(10) << " : "
              << value.ToCentiPawns() << std::setw(10) << msec << " : "
              << turn.ToChessFormat() << std::endl;
    //  << turn.ToChessFormat() << std::endl;
  }
  Score::GetStaticValue({}, Color::kWhite, GameStage{});
}
