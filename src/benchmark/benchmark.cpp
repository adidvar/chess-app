#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "computer.hpp"
#include "itdeepening.hpp"

std::vector<std::pair<const char*, float>> stockfish_result{
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 0.17},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1", -7.24},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RN1QKBNR w KQkq - 0 1", -2.71},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R1BQKBNR w KQkq - 0 1", -2.46},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/1NBQKBNR w Kkq - 0 1", -3.40},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1", -0.40},
    {"rnbqkbnr/pppppppp/8/8/8/8/PP1PPPPP/RNBQKBNR w KQkq - 0 1", -0.44},
    {"rnbqkbnr/pppppppp/8/8/8/8/1PPPPPPP/RNBQKBNR w KQkq - 0 1", -0.27}

};

int main() {
  float delta = 0;

  auto begin = std::chrono::high_resolution_clock::now();
  for (auto pair : stockfish_result) {
    TTable table;
    ItDeepening cmp(Color::kWhite);
    cmp.SetTTable(&table);

    float value = cmp.GetValue(BitBoard{pair.first}, 8).ToCentiPawns();
    auto stat = cmp.GetStatistics();
    float error = value - pair.second;

    std::cout << pair.first << ":::::" << value << "::::" << pair.second
              << std::endl;

    delta += error * error;
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Time: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                     begin)
                   .count()
            << std::endl;

  std::cout << std::endl;
  std::cout << "Delta with stockfish: " << sqrt(delta) << std::endl;

  Statistics statistics;

  for (auto pair : stockfish_result) {
    ItDeepening cmp(Color::kWhite);
    TTable table;
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    cmp.GetValue(BitBoard(pair.first), 6);
    statistics += cmp.GetStatistics();
    std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  }
  std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  statistics.Clear();

  TTable table;

  begin = std::chrono::high_resolution_clock::now();

  for (int d = 1; d <= 10; d++) {
    ItDeepening cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);

    std::string fen{"r1r3k1/pppb1ppp/8/2bN4/2Pp4/1P6/P1KQB2q/R4R2 w - - 0 1"};
    auto value = cmp.GetValue(BitBoard{fen}, d);
    statistics += cmp.GetStatistics();
    auto turn = cmp.GetTurn(BitBoard{fen}, d);

    auto end = std::chrono::high_resolution_clock::now();
    auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
            .count();

    std::cout << std::setw(8) << d << ": " << std::setw(8)
              << statistics.GetMainNode() << "  :  " << std::setw(10)
              << value.ToCentiPawns() << " : " << std::setw(10) << msec << " : "
              << turn.ToChessFormat() << std::endl;
  }
  return 0;
}
