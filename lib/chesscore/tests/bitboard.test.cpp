#include <bitboard.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BitBoard tests", "[bitboard]") {
  SECTION("Test of TestColor GetFigure GetColor") {
    BitBoard board;
    for (uint8_t i = 0; i < 64; i++) {
        for (uint8_t figure = 1; figure < 7; figure++) {
            for (int8_t color = -1; color <= 1; color += 2) {
                board = board.set(i, figure * color);

                REQUIRE(board.get(i) == figure * color);
            }
        }
    }
  }

  SECTION("Test of TestEmp") {
    BitBoard board;
    for (int i = 0; i < 64; i++) {
        board = board.set(i, Figure::Empty);
        REQUIRE(board.get(i) == Figure::Empty);
    }
  }

  SECTION("Test of Swap") {
    BitBoard board;
    for (size_t i = 0; i < 64; i++) {
      for (size_t j = 0; j < 64; j++) {
        if (i == j) continue;

        board = board.set(i, Figure::WPawn);
        board = board.set(j, Figure::BKnight);
        board = board.swap(i, j);
        REQUIRE(board.get(j) == Figure::WPawn);
        REQUIRE(board.get(i) == Figure::BKnight);
      }
    }
  }
  /*
  SECTION("Test of ExecuteTurn GenerateSubBoards TestTurn")
  {
      BitBoard board;
      auto subboards = board.generateSubBoards(Color::White);

      for (size_t i = 0; i < subboards.size(); i++) {
          BitBoard sub = board;
          sub = sub.executeTurn(subboards[i].getTurn());
          REQUIRE(sub.fen() == subboards[i].fen());
      }
  }
  SECTION("Test of el passant") {
    BitBoard board;
    REQUIRE(board.executeTurn(Turn(Position(51), Position(35))));
    REQUIRE(board.LastPawnMove() == Position(43));
    REQUIRE(board.executeTurn(Turn(Position(9), Position(17))));
    REQUIRE(board.LastPawnMove() == Position());
    board = BitBoard();
    board.SkipMove();
    REQUIRE(board.executeTurn(Turn(Position(9), Position(25))));
    REQUIRE(board.LastPawnMove() == Position(17));
    REQUIRE(board.executeTurn(Turn(Position(57), Position(42))));
    REQUIRE(board.LastPawnMove() == Position());
  }
  SECTION("Test of castling") {
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(60), Position(62))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(60), Position(58))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(60), Position(52))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(56), Position(48))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(63), Position(55))));
      REQUIRE(board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(56), Position(0))));
      REQUIRE(!board.RookingFlags().white_ooo);
      REQUIRE(!board.RookingFlags().black_ooo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(63), Position(7))));
      REQUIRE(!board.RookingFlags().black_oo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(4), Position(6))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(4), Position(2))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(4), Position(12))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(0), Position(8))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(7), Position(15))));
      REQUIRE(board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().black_oo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(0), Position(56))));
      REQUIRE(!board.RookingFlags().black_ooo);
      REQUIRE(!board.RookingFlags().white_ooo);
    }
    {
      BitBoard board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
      REQUIRE(board.executeTurn(Turn(Position(7), Position(63))));
      REQUIRE(!board.RookingFlags().black_oo);
      REQUIRE(!board.RookingFlags().white_oo);
    }
  }
*/
}

TEST_CASE("BitBoard fen tests", "[bitboard][fen]") {
  REQUIRE(BitBoard("8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 0").fen() ==
          "8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 0").fen() ==
          "8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/2p5/8/2R1P3/8/8/8 w - - 0 0").fen() ==
          "8/8/2p5/8/2R1P3/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("2q5/8/8/8/8/8/4P3/3K4 w - - 0 0").fen() ==
          "2q5/8/8/8/8/8/4P3/3K4 w - - 0 0");
  REQUIRE(BitBoard("8/8/8/4pP2/8/8/8/8 w - e6 0 0").fen() ==
          "8/8/8/4pP2/8/8/8/8 w - e6 0 0");
  REQUIRE(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 0 0").fen() ==
          "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 0 0");
  REQUIRE(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 0").fen() ==
          "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 0");
  REQUIRE(BitBoard("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 0")
              .fen() ==
          "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 0 0");
  REQUIRE(
      BitBoard(
          "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 0 0")
          .fen() ==
      "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 0 0");
  REQUIRE(BitBoard(
              "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 0 0")
              .fen() ==
          "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 0 0");
  REQUIRE(BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 0 0")
              .fen() ==
          "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 0 0");
  REQUIRE(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4").fen() ==
          "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 0 0");
  REQUIRE(BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 0 0")
              .fen() ==
          "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 0 0");
  REQUIRE(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/"
                   "R4RK1 w - - 0 10")
              .fen() ==
          "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - "
          "0 0");
  REQUIRE(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1").fen() ==
          "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 0");
  REQUIRE(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1").fen() ==
          "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1").fen() ==
          "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 0");
  REQUIRE(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1").fen() ==
          "5k2/8/8/8/8/8/8/4K2R w K - 0 0");
  REQUIRE(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1").fen() ==
          "3k4/8/8/8/8/8/8/R3K3 w Q - 0 0");
  REQUIRE(BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1").fen() ==
          "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 0");
  REQUIRE(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1").fen() ==
          "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 0");
  REQUIRE(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1").fen() ==
          "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 0");
  REQUIRE(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1").fen() ==
          "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 0");
  REQUIRE(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1").fen() ==
          "4k3/1P6/8/8/8/8/K7/8 w - - 0 0");
  REQUIRE(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1").fen() ==
          "8/P1k5/K7/8/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1").fen() ==
          "K1k5/8/P7/8/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1").fen() ==
          "8/k1P5/8/1K6/8/8/8/8 w - - 0 0");
  REQUIRE(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1").fen() ==
          "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 0");
}

static size_t Counter(BitBoard board, size_t depth)
{
    if (!depth)
        return 1;
    size_t counter = 0;
    Turn turns[216];
    bool is_mate;
    int count = board.getTurns(board.side(), turns, is_mate);
    for (int i = 0; i < count; i++) {
      counter += Counter(board.executeTurn(board.side(), turns[i]), depth - 1);
    }
    return counter;
}

static bool MateTest(BitBoard board) {
  Turn turns[216];
  bool is_mate;
  int count = board.getTurns(board.side(), turns, is_mate);

  return is_mate && count == 0;
}

TEST_CASE("BitBoard generation tests", "[bitboard][generation]") {
    REQUIRE(Counter(BitBoard(), 1) == 20);
    REQUIRE(Counter(BitBoard(), 2) == 400);
    REQUIRE(Counter(BitBoard(), 3) == 8902);
    REQUIRE(Counter(BitBoard(), 4) == 197281);

    REQUIRE(MateTest(BitBoard{"Q3k3/Q7/8/8/8/8/8/3K4 b - - 1 1"}) == true);
    REQUIRE(MateTest(BitBoard{"4k3/Q7/8/8/8/8/8/3K3Q b - - 1 1"}) == false);
}

TEST_CASE("BitBoard generation advanced tests", "[bitboard][generation]") {
    REQUIRE(Counter(BitBoard("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"), 1) == 8);
    REQUIRE(Counter(BitBoard("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"), 1) == 8);
    REQUIRE(Counter(BitBoard("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"), 1)
            == 19);
    REQUIRE(Counter(BitBoard("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/"
                             "PPPBBPPP/R3K2R b KQkq - 3 2"),
                    1)
            == 5);

    REQUIRE(Counter(BitBoard("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2"), 1)
            == 44);
    REQUIRE(Counter(BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"), 1) == 39);
    REQUIRE(Counter(BitBoard("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"), 1) == 9);
    REQUIRE(Counter(BitBoard("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"), 3)
            == 62379);
    REQUIRE(Counter(BitBoard("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/"
                             "P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),
                    3)
            == 89890);
    REQUIRE(Counter(BitBoard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"), 6) == 1134888);
    REQUIRE(Counter(BitBoard("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"), 6) == 1015133);
    REQUIRE(Counter(BitBoard("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"), 6) == 1440467);
    REQUIRE(Counter(BitBoard("5k2/8/8/8/8/8/8/4K2R w K - 0 1"), 6) == 661072);
    REQUIRE(Counter(BitBoard("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"), 6) == 803711);
    REQUIRE(Counter(BitBoard("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"), 4) == 1274206);
    REQUIRE(Counter(BitBoard("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"), 4) == 1720476);
    REQUIRE(Counter(BitBoard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"), 6) == 3821001);
    REQUIRE(Counter(BitBoard("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"), 5) == 1004658);
    REQUIRE(Counter(BitBoard("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"), 6) == 217342);
    REQUIRE(Counter(BitBoard("8/P1k5/K7/8/8/8/8/8 w - - 0 1"), 6) == 92683);
    REQUIRE(Counter(BitBoard("K1k5/8/P7/8/8/8/8/8 w - - 0 1"), 6) == 2217);
    REQUIRE(Counter(BitBoard("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"), 7) == 567584);
    REQUIRE(Counter(BitBoard("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"), 4) == 23527);
}
