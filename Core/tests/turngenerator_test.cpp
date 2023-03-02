#include <catch2/catch_test_macros.hpp>
#include <board.h>

TEST_CASE( "TurnGenerator ", "[generator]" )
{
   REQUIRE(Board("8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 1").GenerateTurns().size() == 5);
   REQUIRE(Board("8/8/2p1p3/1p3p2/3N4/1p3p2/2p1p3/8 w - - 0 1").GenerateTurns().size() == 8);
   REQUIRE(Board("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1").GenerateTurns().size() == 8);
   REQUIRE(Board("8/8/2p5/8/2R1P3/8/8/8 w - - 0 1").GenerateTurns().size() == 9);
   REQUIRE(Board("2q5/8/8/8/8/8/4P3/3K4 w - - 0 1").GenerateTurns().size() == 4);
   REQUIRE(Board("8/8/8/4pP2/8/8/8/8 w - e6 0 1").GenerateTurns().size() == 2);
}

static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    auto moves = board.GenerateTurns();
    size_t counter = 0;
    for(auto move : moves){
        Board copy(board);
        copy.ExecuteTurn(move);
        counter += MovesCounter(copy,depth-1);
    }
    return counter;
}

TEST_CASE( "Test of Chess rules", "[chesstree]" )
{
   REQUIRE(MovesCounter(Board(),1) == 20);
   REQUIRE(MovesCounter(Board(),2) == 400);
   REQUIRE(MovesCounter(Board(),3) == 8902);
   REQUIRE(MovesCounter(Board(),4) == 197281);
   REQUIRE(MovesCounter(Board("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"),1) == 8);
   REQUIRE(MovesCounter(Board("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"),1) == 8);
   REQUIRE(MovesCounter(Board("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"),1) == 19);
   REQUIRE(MovesCounter(Board("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2"),1) == 5);
   REQUIRE(MovesCounter(Board("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2"),1) == 44);
   REQUIRE(MovesCounter(Board("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"),1) == 39);
   REQUIRE(MovesCounter(Board("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"),1) == 9);
   REQUIRE(MovesCounter(Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),3) == 62379);
   REQUIRE(MovesCounter(Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),3) == 89890);
   REQUIRE(MovesCounter(Board("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"),6) == 1134888);
   REQUIRE(MovesCounter(Board("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"),6) == 1015133);
   REQUIRE(MovesCounter(Board("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"),6) == 1440467);
   REQUIRE(MovesCounter(Board("5k2/8/8/8/8/8/8/4K2R w K - 0 1"),6) == 661072);
   REQUIRE(MovesCounter(Board("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"),6) == 803711);
   REQUIRE(MovesCounter(Board("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"),4) == 1274206);
   REQUIRE(MovesCounter(Board("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"),4) == 1720476);
   REQUIRE(MovesCounter(Board("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"),6) == 3821001);
   REQUIRE(MovesCounter(Board("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"),5) == 1004658);
   REQUIRE(MovesCounter(Board("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"),6) == 217342);
   REQUIRE(MovesCounter(Board("8/P1k5/K7/8/8/8/8/8 w - - 0 1"),6) == 92683);
   REQUIRE(MovesCounter(Board("K1k5/8/P7/8/8/8/8/8 w - - 0 1"),6) == 2217);
   REQUIRE(MovesCounter(Board("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"),7) == 567584);
   REQUIRE(MovesCounter(Board("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"),4) == 23527);
}
