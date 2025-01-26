#include <chessai/alphabeta.hpp>

/*
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "score.hpp"

static bool TestMateFind(const char* fen, int depth) {
  TTable table;
  AlphaBeta ab(BitBoard{fen}, Color::kWhite);
  ab.SetStopFlag(nullptr);
  ab.SetTTable(&table);

  auto result = ab.GetValue(depth + 1);
  return result.IsCheckMate() && abs(result.GetTurnsToCheckMate()) == depth;
}

TEST_CASE("Testing of mate search in alpha beta", "[alphabeta][ai]") {
  SECTION("Depth 1") {
    REQUIRE(TestMateFind(
        "4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32", 1));
    REQUIRE(TestMateFind(
        "4rb2/3qrk2/1p1p1n2/7p/P2P4/4R2P/1BQN1P2/1K4R1 w - - 3 39", 1));
    REQUIRE(TestMateFind(
        "r1bqkbnr/pp2pp1p/3p2p1/2p5/3nP3/2N3PP/PPP1NP2/R1BQKB1R b KQkq - 1 6",
        1));
    REQUIRE(TestMateFind(
        "r2q1rk1/1bppb1pp/n3p3/5P2/2BP1P2/4P3/1PPnQK1P/R1B3NR b - - 1 12", 1));
  }
  SECTION("Depth 3") {
    REQUIRE(TestMateFind(
        "4r3/1pp2rbk/6pn/4n3/P3BN1q/1PB2bPP/8/2Q1RRK1 b - - 0 31", 3));
    REQUIRE(TestMateFind(
        "r2k1b1r/p1ppq2p/np3np1/5p2/1PPP4/P3PQ2/3N1PPP/R1B1K2R w KQ - 1 13",
        3));
    REQUIRE(TestMateFind(
        "rn3r2/p2q1pBk/1p2p3/3pP3/P1bNnQ2/5NP1/1P3PBP/R3b1K1 w - - 1 19", 3));
    REQUIRE(TestMateFind(
        "rn3r1k/p2q1p2/1p2p2p/3pP3/PbbNRQ2/5NP1/1P3PBP/R5K1 w - - 1 19", 3));
  }
  SECTION("Depth 5") {
    REQUIRE(
        TestMateFind("4r1k1/4r1p1/8/p2R1P1K/5P1P/1QP3q1/1P6/3R4 b - - 0 1", 5));
    REQUIRE(TestMateFind(
        "r1bqk2r/2ppb1p1/n3P2p/8/2B1nP2/4P3/1PPP3P/RNBQK1NR w KQkq - 0 10", 5));
    REQUIRE(TestMateFind(
        "rn3r1k/p3qp2/bp2p2p/3pP3/P2NRQ2/1Pb2NPP/5PB1/2R3K1 w - - 1 22", 5));
    REQUIRE(TestMateFind(
        "rn5r/p2q1p1k/bp2N2p/3pP3/Pb3Q2/1P3NP1/5PB1/R3R1K1 w - - 1 23", 5));
  }
  SECTION("Depth 7") {
    /*
  REQUIRE(TestMateFind(
      "3qr2k/1p3rbp/2p3p1/p7/P2pBNn1/1P3n2/6P1/B1Q1RR1K b - - 1 30", 7));
  REQUIRE(TestMateFind(
      "r1bqk1nr/pp1p2bp/4n3/2p1Npp1/5P2/2N1P1PP/PPP5/1RBQKB1R w Kkq - 0 10",
      7));
  REQUIRE(TestMateFind(
      "r2q1rk1/2p3pp/b3P3/3p3Q/3PnP1b/4P3/1PPN3P/R1BK2NR b - - 1 15", 7));
  REQUIRE(TestMateFind(
      "1rbq1rk1/R5pp/2p2P2/2np4/2P1pPB1/4P1b1/1P2Q2P/1NB2KNR w - - 1 19", 7));
  }
}

TEST_CASE("Testing of fast search exit", "[itdeepening][ai]") {
  AlphaBeta ab(BitBoard{}, Color::kWhite);
  TTable table;

  std::atomic_bool flag = true;
  ab.SetStopFlag(&flag);
  ab.SetTTable(&table);

  CHECK_THROWS_AS(ab.GetValue(10), SearchExitException);
}

TEST_CASE("Test of state of turn search", "[alphabeta][ai]") {
  BitBoard board{};

  TTable table;
  AlphaBeta ab(board, Color::kWhite);
  ab.SetStopFlag(nullptr);
  ab.SetTTable(&table);

  int depth = 6;

  REQUIRE(!ab.GetTurn().Valid());
  REQUIRE(ab.FindPV().size() == 0);

  auto value = ab.GetValue(depth);
  auto turn = ab.GetTurn();
  auto pv = ab.FindPV();
  auto turn1 = ab.GetTurn();

  REQUIRE(turn == turn1);
  REQUIRE(pv.size() == depth);
}

TEST_CASE("Test of research on same object", "[alphabeta][ai]") {
  BitBoard board{};

  TTable table1;
  TTable table2;
  AlphaBeta ab1(board, Color::kWhite);
  AlphaBeta ab2(board, Color::kWhite);

  ab1.SetStopFlag(nullptr);
  ab1.SetTTable(&table1);

  ab2.SetStopFlag(nullptr);
  ab2.SetTTable(&table2);

  ab1.GetValue(4);
  ab1.GetValue(6);
  auto turn1 = ab1.GetTurn();

  ab2.GetValue(6);
  auto turn2 = ab2.GetTurn();

  REQUIRE(turn1 == turn2);
}

TEST_CASE("Test of research on same object with different depth",
          "[alphabeta][ai]") {
  BitBoard board{};

  TTable table1;
  AlphaBeta ab1(board, Color::kWhite);

  ab1.SetStopFlag(nullptr);
  ab1.SetTTable(&table1);

  for (int i = 0; i < 7; i++) {
    AlphaBeta ab2(board, Color::kWhite);
    TTable table2;
    ab2.SetStopFlag(nullptr);
    ab2.SetTTable(&table2);

    ab1.GetValue(i);
    auto turn1 = ab1.GetTurn();

    ab2.GetValue(i);
    auto turn2 = ab2.GetTurn();

    REQUIRE(turn1 == turn2);
  }
}

TEST_CASE("Testing of alpha beta move correctness", "[alphabeta][minmax][ai]") {
  {
    AlphaBeta ab(BitBoard("5q1k/8/8/8/8/8/8/5Q1K w - - 0 1"), Color::kWhite);
    ab.GetValue(4);
    REQUIRE(ab.GetTurn().ToChessFormat() == "f1f8");
  }
  {
    AlphaBeta ab(BitBoard("5q1k/8/8/8/8/8/8/5Q1K b - - 0 1"), Color::kWhite);
    ab.GetValue(4);
    REQUIRE(ab.GetTurn().ToChessFormat() == "f8f1");
  }
}
*/
