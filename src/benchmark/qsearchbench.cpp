#include "qsearchbench.hpp"

#include "benchmark.hpp"
#include "evaluate.hpp"
#include "itdeepening.hpp"
#include "pvs.hpp"
#include "qsearch.hpp"

std::vector<const char *> qsearch_tests{
    "rnbqkbnr/pppp1ppp/8/4p3/3P4/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1",
    "r1bqkbnr/pppp1ppp/2n5/4p3/3P4/5N2/PPP1PPPP/RNBQKB1R w KQkq - 0 1",
    "r1bqk1nr/pppp1ppp/2n2b2/4p3/3P4/2B2N2/PPP1PPPP/RN1QKB1R w KQkq - 0 1",
    "r1bqk2r/pppp1ppp/2n2bn1/4p3/3P4/1NB2N2/PPP1PPPP/R2QKB1R w KQkq - 0 1",
    "r1b1k2r/pppp1ppp/1qn2bn1/4p3/3P4/1NB2NB1/PPP1PPPP/R2QK2R w KQkq - 0 1",
    "2b1k2r/pppp1ppp/1qn2bn1/r3p3/3P3R/1NB2NB1/PPP1PPPP/R2QK3 w Qk - 0 1",
    "4k3/pppp1ppp/1qnb1bn1/r3p2r/3P3R/1NB2NB1/PPP1PPPP/R2QK3 w Q - 0 1",
    "4k3/pppp1ppp/1qnb1bn1/r3p2r/1R1P3R/1NB2NB1/PPP1PPPP/3QK3 w - - 0 1",
    "4k3/pppp1ppp/2nb1bn1/r1q1p1r1/1R1P3R/1NB1QNB1/PPP1PPPP/4K3 w - - 0 1",
    "8/p1ppkppp/2nb1bn1/rpq1p1r1/1R1P1P1R/1NB1QNB1/PPPKP1PP/8 w - - 0 1",
    "8/p1p1kppp/2nb1bn1/rpq1p1r1/PRpP1P1R/1NB1QNB1/1PPKP1PP/8 w - - 0 1",
    "8/p1p1kppp/2nb1bn1/rpq1p1rP/PRpP1P1R/1NB1QNB1/1PPKP1P1/8 w - - 0 1",
    "8/p1p1kppp/2nb1bn1/rpq1p1rP/PRpP1P1R/1NBPQNB1/1PPK2P1/8 w - - 0 1",
    "8/p1p1kppp/1Pnb1bn1/rpq1p1rP/PRpP1P1R/1NBPQNB1/2PK2P1/8 w - - 0 1",
    "8/p1p1kppp/1Pnb1bn1/rpq1p1rP/PRpPQP1R/1NBP1NB1/2PK2P1/8 w - - 0 1",
};

BenchmarkPrint::Test GetTest(std::string fen) {
  return [fen](Statistics &stat, Score &score, Turn &turn, int &depth) {
    QSearch search(BitBoard{}, Color::kWhite);
    auto board = BitBoard{fen};
    search.SetStopFlag(nullptr);
    score = search.QuiescenceSearch(BitBoardTuple(board, board.Hash(), {}));
    depth = 0;
    turn = {};
    stat = search.GetStatistics();
  };
};

BenchmarkPrint::Test GetDeepTest(std::string fen) {
  return [fen](Statistics &stat, Score &score, Turn &turn, int &depth) {
    ItDeepening<PVS> search(Color::kWhite);
    auto board = BitBoard{fen};
    search.SetStopFlag(nullptr);
    score = search.GetValue(board, 6);
    depth = 0;
    turn = {};
    stat = search.GetStatistics();
  };
};

BenchmarkPrint::Test GetStaticTest(std::string fen) {
  return [fen](Statistics &stat, Score &score, Turn &turn, int &depth) {
    auto board = BitBoard{fen};
    score = Score::Value(board, Color::kWhite);
    depth = 0;
    turn = {};
  };
};

void QSearchBenchmark() {
  BenchmarkPrint print("QSearch");

  auto index = 0;
  for (auto test : qsearch_tests) {
    print.Add(GetTest(test), index);
    // print.Add(GetDeepTest(test), index);
    // print.Add(GetStaticTest(test), index);
    index++;
  };

  print.RunAndPrint();
}
