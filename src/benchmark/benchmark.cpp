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

  for (auto pair : stockfish_result) {
    Computer cmp(Color::kWhite);
    cmp.SetBoard(BitBoard(pair.first));
    cmp.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds{500});
    cmp.Stop();
    auto stat = cmp.GetStatistics();

    float value = cmp.GetValue().ToCentiPawns();
    float error = value - pair.second;

    std::cout << pair.first << ":::::" << stat.GetMainNode() << "::::" << error
              << std::endl;

    delta += error * error;
  }

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
  }
  std::cout << "NodesCounter: " << statistics.GetMainNode() << std::endl;

  TTable table;
  statistics.Clear();
  for (int d = 1; d <= 7; d++) {
    AlphaBeta cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    auto value = cmp.GetValue(
        BitBoard{"2b1rrk1/pp4Pp/2pp4/4p3/2P4Q/2PPP3/P7/2KR1B1R b K - 0 1"}, d);
    statistics += cmp.GetStatistics();
    std::cout << d << ": " << statistics.GetMainNode() << ": "
              << value.ToCentiPawns() << std::endl;
    auto pv = cmp.FindPV(
        {"2b1rrk1/pp4Pp/2pp4/4p3/2P4Q/2PPP3/P7/2KR1B1R b K - 0 1"}, d);
    for (auto p : pv) std::cout << p.ToChessFormat() << " ";
    std::cout << std::endl;
  }

  table.Clear();

  for (int d = 7; d >= 2; d--) {
    AlphaBeta cmp(Color::kWhite);
    cmp.SetTTable(&table);
    cmp.SetStopFlag(nullptr);
    auto value = cmp.GetValue(
        BitBoard{"2b1rrk1/pp4Pp/2pp4/4p3/2P4Q/2PPP3/P7/2KR1B1R b K - 0 1"}, d);
    statistics += cmp.GetStatistics();
    std::cout << d << ": " << statistics.GetMainNode() << ": "
              << value.ToCentiPawns() << std::endl;
    auto pv = cmp.FindPV(
        {"2b1rrk1/pp4Pp/2pp4/4p3/2P4Q/2PPP3/P7/2KR1B1R b K - 0 1"}, d);
    for (auto p : pv) std::cout << p.ToChessFormat() << " ";
    std::cout << std::endl;
  }

  return 0;
}
