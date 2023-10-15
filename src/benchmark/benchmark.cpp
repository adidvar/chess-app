#include <cmath>
#include <iostream>
#include <vector>

#include "computer.hpp"
#include "itdeepening.hpp"

std::vector<std::pair<const char*, float>> stockfish_result{
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 0.17},
    {"rnbqkbnr/pppppppp/8/8/2P2P2/1BN1B3/PP1PP1PP/R2QK1NR w KQkq - 0 1", 0.91},
    {"r2qk2r/pppppppp/b1n2n1b/8/2P2P2/1BN1B3/PP1PP1PP/R2QK1NR w KQkq - 0 1",
     0.17},
    {"r2qk2r/ppp1pppp/b1n2n1b/4p3/2P2P2/1BN1BN2/PP1PP1PP/R2Q1RK1 w Qkq - 0 1",
     1.04},
    {"r2qk2r/ppp1pppp/b1n2n1b/4p3/2P2P2/1B3N2/PP1PP1PP/R2Q1RK1 w Qkq - 0 1",
     -6.24},
    {"3qk3/ppp1pppp/b1n2n1b/4p3/2P2P2/1B3N2/PP1PP1PP/R2Q1RK1 w Q - 0 1", 2.81},
    {"3qk3/p1p1pppp/b1n2n1b/4p3/2P2P2/1B3N2/PP1P2P1/R2Q1RK1 w Q - 0 1", 3.83},
    {"3qk3/7p/b1n2n1b/8/8/1B3N2/P7/R2Q1RK1 w Q - 0 1", 5.05},
    {"3qk3/7p/b1n2n1b/8/8/1B3N2/P7/R2Q2K1 w Q - 0 1", -0.3},
    {"3qk3/7p/P1nb1n1b/8/2B5/5N2/8/RQ4K1 w Q - 0 1", 1.40},
};

int main() {
  float delta = 0;

  auto begin = std::chrono::high_resolution_clock::now();
  for (auto pair : stockfish_result) {
    TTable table;
    ItDeepening cmp(Color::kWhite);
    cmp.SetTTable(&table);

    float value = cmp.GetValue(BitBoard{pair.first}, 6).ToCentiPawns();
    auto stat = cmp.GetStatistics();
    float error = value - pair.second;

    std::cout << pair.first << ":::::" << stat.GetMainNode() << "::::" << error
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
    cmp.GetValue(BitBoard(pair.first), 8);
    statistics += cmp.GetStatistics();
    std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  }
  std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;
  return 0;
}
