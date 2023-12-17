#include "qsearchbench.hpp"

#include "benchmark.hpp"
#include "evaluate.hpp"
#include "itdeepening.hpp"
#include "qsearch.hpp"

/*
static std::vector<const char *> pvssearch_tests{
    "startpos",
    "r3k2r/ppp1bppp/q1np1n2/1B2p3/3P2b1/2N1PN2/PPPBQPPP/R4RK1 w Qkq - 0 1",
    "r3k2r/8/q7/8/8/8/3Q4/R4RK1 w Qkq - 0 1"};

static BenchmarkPrint::Test GetTest(std::string fen) {
  return [fen](Statistics &stat, Score &score, Turn &turn, int &depth) {
    ItDeepening<PVS> search(Color::kWhite);
    auto board = BitBoard{fen};
    search.SetStopFlag(nullptr);
    score = search.GetValue(board, 7);
    depth = 7;
    turn = {};
    stat = search.GetStatistics();
  };
};
*/

void PVSSearchBenchmark() {
  /*
  BenchmarkPrint print("PVSSearch");

  auto index = 0;
  for (auto test : pvssearch_tests) {
    print.Add(GetTest(test), index);
    index++;
  };

  print.RunAndPrint();
*/
}
