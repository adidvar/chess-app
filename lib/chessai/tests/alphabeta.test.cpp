#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chessai/computer.hpp>

struct TestFeedBack : public IFeedBack {
  void sendBestMove(Turn bestMove) override {
    turn = bestMove;
    ready = true;
  }
  Turn turn;
  std::atomic_bool ready = false;
};

static std::string testComputer(const char* fen, int depth) {
  SearchSettings settings;
  auto back = std::make_shared<TestFeedBack>();
  settings.depth = depth;
  settings.feedback = std::dynamic_pointer_cast<IFeedBack>(back);
  settings.board = BitBoard(fen);

  Computer computer;

  computer.Start(settings);

  while (back->ready != true) std::this_thread::yield();

  return back->turn.toString();
}

TEST_CASE("Search logic testing", "[search]") {
  SECTION("Well turns testing") {
    REQUIRE(true);
    REQUIRE(testComputer(
                "rnb1kbnr/pppppppp/8/8/2q5/3P4/PPP1PPPP/RNBQKBNR w KQkq - 0 1",
                1) == "d3c4");
    REQUIRE(testComputer(
                "rnbqkbnr/pppp1ppp/4p3/3Q4/8/8/PPPPPPPP/RNB1KBNR b KQkq - 0 1",
                1) == "e6d5");
    REQUIRE(testComputer(
                "rnbp1bnr/ppkppppp/2q5/8/8/1R6/PPPPPPPP/1NBQKBNR w KQkq - 0 1",
                3) == "b3c3");
    REQUIRE(testComputer(
                "rnbqkbn1/pppppppp/4r3/8/8/3Q4/PPPKPPPP/RNB2BNR b KQkq - 0 1",
                3) == "e6d6");
  }
  SECTION("Checkmate turns testing") {
    REQUIRE(true);
    REQUIRE(testComputer(
                "4r2k/1p3rbp/2p1N1p1/p3n3/P2NB1nq/1P6/4R1P1/B1Q2RK1 b - - 4 32",
                2) == "h4h2");
    REQUIRE(testComputer(
                "4r3/1pp2rbk/6pn/4n3/P3BN1q/1PB2bPP/8/2Q1RRK1 b - - 0 31", 3) ==
            "h4g3");
    REQUIRE(testComputer("4r1k1/4r1p1/8/p2R1P1K/5P1P/1QP3q1/1P6/3R4 b - - 0 1",
                         4) == "g7g6");
  }

  SECTION("Good turns testing") { REQUIRE(true); }
}
