#include <catch2/catch_test_macros.hpp>
#include <board.hpp>

TEST_CASE( "FenTest ", "[Fen]" )
{
   REQUIRE(Board("8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 1").Fen() == "8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 1");
   REQUIRE(Board("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1").Fen() == "8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1");
   REQUIRE(Board("8/8/2p5/8/2R1P3/8/8/8 w - - 0 1").Fen() == "8/8/2p5/8/2R1P3/8/8/8 w - - 0 1");
   REQUIRE(Board("2q5/8/8/8/8/8/4P3/3K4 w - - 0 1").Fen() == "2q5/8/8/8/8/8/4P3/3K4 w - - 0 1");
   REQUIRE(Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1").Fen() == "8/8/8/4pP2/8/8/8/8 w - e6 0 1");
   REQUIRE(Board("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2").Fen() == "r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2");
   REQUIRE(Board("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3").Fen() == "8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3");
   REQUIRE(Board("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2").Fen() == "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2");
   REQUIRE(Board("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2").Fen() == "r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2");
   REQUIRE(Board("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2").Fen() == "2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2");
   REQUIRE(Board("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9").Fen() == "rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9");
   REQUIRE(Board("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4").Fen() == "2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4");
   REQUIRE(Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8").Fen() == "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
   REQUIRE(Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10").Fen() == "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
   REQUIRE(Board("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1").Fen() == "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1");
   REQUIRE(Board("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1").Fen() == "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1");
   REQUIRE(Board("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1").Fen() == "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
   REQUIRE(Board("5k2/8/8/8/8/8/8/4K2R w K - 0 1").Fen() == "5k2/8/8/8/8/8/8/4K2R w K - 0 1");
   REQUIRE(Board("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1").Fen() == "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
   REQUIRE(Board("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1").Fen() == "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
   REQUIRE(Board("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1").Fen() == "r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
   REQUIRE(Board("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1").Fen() == "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
   REQUIRE(Board("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1").Fen() == "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
   REQUIRE(Board("4k3/1P6/8/8/8/8/K7/8 w - - 0 1").Fen() == "4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
   REQUIRE(Board("8/P1k5/K7/8/8/8/8/8 w - - 0 1").Fen() == "8/P1k5/K7/8/8/8/8/8 w - - 0 1");
   REQUIRE(Board("K1k5/8/P7/8/8/8/8/8 w - - 0 1").Fen() == "K1k5/8/P7/8/8/8/8/8 w - - 0 1");
   REQUIRE(Board("8/k1P5/8/1K6/8/8/8/8 w - - 0 1").Fen() == "8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
   REQUIRE(Board("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1").Fen() == "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
}
