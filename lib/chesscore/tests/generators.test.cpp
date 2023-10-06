#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <board.hpp>
#include <bitboard.hpp>

template<typename Board>
static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    size_t counter = 0;
    for(auto board : board.GenerateSubBoards(board.CurrentColor())){
        counter += MovesCounter(board,depth-1);
    }
    return counter;
}

TEMPLATE_TEST_CASE( "Boards subboards generation test", "[bitboard][board][fen]", BitBoard){
   REQUIRE(TestType("8/8/8/8/6pp/3P1ppP/1P3P2/8 w - - 0 1").GenerateTurns(0).size() == 5);
   REQUIRE(TestType("8/8/2p1p3/1p3p2/3N4/1p3p2/2p1p3/8 w - - 0 1").GenerateTurns(0).size() == 8);
   REQUIRE(TestType("8/8/1p3P2/8/3B4/4p3/5p2/8 w - - 0 1").GenerateTurns(0).size() == 8);
   REQUIRE(TestType("8/8/2p5/8/2R1P3/8/8/8 w - - 0 1").GenerateTurns(0).size() == 9);
   REQUIRE(TestType("2q5/8/8/8/8/8/4P3/3K4 w - - 0 1").GenerateTurns(0).size() == 4);
   REQUIRE(TestType("8/8/8/4pP2/8/8/8/8 w - e6 0 1").GenerateTurns(0).size() == 2);

   REQUIRE(MovesCounter(TestType(),1) == 20);
   REQUIRE(MovesCounter(TestType(),2) == 400);
   REQUIRE(MovesCounter(TestType(),3) == 8902);
}

TEMPLATE_TEST_CASE( "Boards subboards generation advanced test", "[bitboard][board][fen]", BitBoard){
   REQUIRE(MovesCounter(TestType(),4) == 197281);
   REQUIRE(MovesCounter(TestType("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2"),1) == 8);
   REQUIRE(MovesCounter(TestType("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3"),1) == 8);
   REQUIRE(MovesCounter(TestType("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2"),1) == 19);
   REQUIRE(MovesCounter(TestType("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2"),1) == 5);
   REQUIRE(MovesCounter(TestType("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2"),1) == 44);
   REQUIRE(MovesCounter(TestType("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9"),1) == 39);
   REQUIRE(MovesCounter(TestType("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4"),1) == 9);
   REQUIRE(MovesCounter(TestType("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),3) == 62379);
   REQUIRE(MovesCounter(TestType("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"),3) == 89890);
   REQUIRE(MovesCounter(TestType("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"),6) == 1134888);
   REQUIRE(MovesCounter(TestType("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1"),6) == 1015133);
   REQUIRE(MovesCounter(TestType("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1"),6) == 1440467);
   REQUIRE(MovesCounter(TestType("5k2/8/8/8/8/8/8/4K2R w K - 0 1"),6) == 661072);
   REQUIRE(MovesCounter(TestType("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1"),6) == 803711);
   REQUIRE(MovesCounter(TestType("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"),4) == 1274206);
   REQUIRE(MovesCounter(TestType("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1"),4) == 1720476);
   REQUIRE(MovesCounter(TestType("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"),6) == 3821001);
   REQUIRE(MovesCounter(TestType("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1"),5) == 1004658);
   REQUIRE(MovesCounter(TestType("4k3/1P6/8/8/8/8/K7/8 w - - 0 1"),6) == 217342);
   REQUIRE(MovesCounter(TestType("8/P1k5/K7/8/8/8/8/8 w - - 0 1"),6) == 92683);
   REQUIRE(MovesCounter(TestType("K1k5/8/P7/8/8/8/8/8 w - - 0 1"),6) == 2217);
   REQUIRE(MovesCounter(TestType("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"),7) == 567584);
   REQUIRE(MovesCounter(TestType("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1"),4) == 23527);

}

TEMPLATE_TEST_CASE( "Boards benchmark", "[bitboard][benchmark][fen]", BitBoard){

    BENCHMARK("Bench 1") {
        return MovesCounter(TestType("8/k1P5/8/1K6/8/8/8/8 w - - 0 1"),5);
    };

    BENCHMARK("Bench 2") {
        return MovesCounter(TestType("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1"),4);
    };

    BENCHMARK("Bench 3") {
        return MovesCounter(TestType("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1"),4);
    };

    BENCHMARK("Bench 4") {
        return MovesCounter(TestType("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1"),3);
    };
}

template<typename T>
void TestTurn(T board)
{
    auto mysubs = board.GenerateSubBoards(board.CurrentColor());
    auto opsubs = board.GenerateSubBoards(board.OpponentColor());
    auto myturns = board.GenerateTurns(board.CurrentColor());
    auto opturns = board.GenerateTurns(board.OpponentColor());
    REQUIRE(mysubs.size() == myturns.size());
    REQUIRE(opsubs.size() == opturns.size());

    for(size_t i = 0 ; i < mysubs.size() ; ++i)
    {
        T temp(board);
        temp.ExecuteTurn(myturns[i]);
        REQUIRE(temp.Fen() == mysubs[i].Fen());
    }
}

TEMPLATE_TEST_CASE( "Boards moves generation test", "[bitboard][board][fen][turns]", BitBoard){
   (TestTurn(TestType()));
   (TestTurn(TestType("r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2")));
   (TestTurn(TestType("8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3")));
   (TestTurn(TestType("r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2")));
   (TestTurn(TestType("r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2")));
   (TestTurn(TestType("2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2")));
   (TestTurn(TestType("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9")));
   (TestTurn(TestType("2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4")));
   (TestTurn(TestType("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8")));
   (TestTurn(TestType("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10")));
   (TestTurn(TestType("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1")));
   (TestTurn(TestType("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1")));
   (TestTurn(TestType("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1")));
   (TestTurn(TestType("5k2/8/8/8/8/8/8/4K2R w K - 0 1")));
   (TestTurn(TestType("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1")));
   (TestTurn(TestType("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1")));
   (TestTurn(TestType("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1")));
   (TestTurn(TestType("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1")));
   (TestTurn(TestType("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1")));
   (TestTurn(TestType("4k3/1P6/8/8/8/8/K7/8 w - - 0 1")));
   (TestTurn(TestType("8/P1k5/K7/8/8/8/8/8 w - - 0 1")));
   (TestTurn(TestType("K1k5/8/P7/8/8/8/8/8 w - - 0 1")));
   (TestTurn(TestType("8/k1P5/8/1K6/8/8/8/8 w - - 0 1")));
   (TestTurn(TestType("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1")));
}
