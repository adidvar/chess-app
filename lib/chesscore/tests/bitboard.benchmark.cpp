#include "bitboard.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("BitBoard benchmarks", "[benchmark]")
{
    BENCHMARK_ADVANCED("start-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
        BitBoard board;
        std::vector<BitBoard> results;
        results.reserve(120);
        meter.measure([&] { return board.generateSubBoards(results, Color::White); });
    };
    BENCHMARK_ADVANCED("middle-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
        BitBoard board{"r1bqkbnr/1p4p1/1pn2pp1/3P4/4P3/1PN3P1/1P4P1/R1BQKBNR w KQkq - 0 10"};
        std::vector<BitBoard> results;
        results.reserve(120);
        meter.measure([&] { return board.generateSubBoards(results, Color::White); });
    };
    BENCHMARK_ADVANCED("end-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
        BitBoard board{"4k3/7r/1pn2q2/3P4/1R6/1PNQ2P1/r4R2/2B1K3 w - - 0 10"};
        std::vector<BitBoard> results;
        results.reserve(120);
        meter.measure([&] { return board.generateSubBoards(results, Color::White); });
    };
}
