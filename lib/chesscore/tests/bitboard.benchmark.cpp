#include "bitboard.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("BitBoard benchmarks", "[benchmark]")
{
    BENCHMARK_ADVANCED("start-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
      bool is_mate;
      BitBoard board;
      Turn turns[216];
      meter.measure(
          [&] { return board.getTurns(Color::White, turns, is_mate); });
    };
    BENCHMARK_ADVANCED("middle-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
      bool is_mate;
      BitBoard board{
          "r1bqkbnr/1p4p1/1pn2pp1/3P4/4P3/1PN3P1/1P4P1/R1BQKBNR w KQkq - 0 10"};
      Turn turns[216];
      meter.measure(
          [&] { return board.getTurns(Color::White, turns, is_mate); });
    };
    BENCHMARK_ADVANCED("end-pos subboards generation")(Catch::Benchmark::Chronometer meter)
    {
      bool is_mate;
      BitBoard board{"4k3/7r/1pn2q2/3P4/1R6/1PNQ2P1/r4R2/2B1K3 w - - 0 10"};
      Turn turns[216];
      meter.measure(
          [&] { return board.getTurns(Color::White, turns, is_mate); });
    };
}

TEST_CASE("BitBoard make turn", "[benchmark]")
{
    BENCHMARK_ADVANCED("start-pos turn generation")(Catch::Benchmark::Chronometer meter)
    {
      BitBoard board{};
      meter.measure(
          [&] { return board.executeTurn(Color::White, Turn("d2d4")); });
    };
    BENCHMARK_ADVANCED("middle-pos turn generation")(Catch::Benchmark::Chronometer meter)
    {
      BitBoard board{
          "r1bqkbnr/1p4p1/1pn2pp1/3P4/4P3/1PN3P1/1P4P1/R1BQKBNR w KQkq - 0 10"};
      meter.measure(
          [&] { return board.executeTurn(Color::White, Turn("d1e2")); });
    };
    BENCHMARK_ADVANCED("end-pos turn generation")(Catch::Benchmark::Chronometer meter)
    {
      BitBoard board{"4k3/7r/1pn2q2/3P4/1R6/1PNQ2P1/r4R2/2B1K3 w - - 0 10"};
      meter.measure(
          [&] { return board.executeTurn(Color::White, Turn("d3e4")); });
    };
}

static size_t Counter(const BitBoard &board, size_t depth)
{
    if (depth == 0)
        return 1;
    size_t counter = 0;
    bool is_mate;
    Turn turns[216];
    int count = board.getTurns(board.getCurrentSide(), turns, is_mate);
    if (depth == 1)
        return count;
    for (int i = 0; i < count; i++) {
        counter += Counter(board.executeTurn(board.getCurrentSide(), turns[i]), depth - 1);
    }
    return counter;
}

/*
TEST_CASE("BitBoard perft", "[benchmark]")
{
    BENCHMARK_ADVANCED("start-pos turn
generation")(Catch::Benchmark::Chronometer meter)
    {
        BitBoard board{};
        meter.measure([&] { return Counter(board, 4); });
    };
    BENCHMARK_ADVANCED("middle-pos turn
generation")(Catch::Benchmark::Chronometer meter)
    {
        BitBoard board{"r1bqkbnr/1p4p1/1pn2pp1/3P4/4P3/1PN3P1/1P4P1/R1BQKBNR w
KQkq - 0 10"}; meter.measure([&] { return Counter(board, 4); });
    };
    BENCHMARK_ADVANCED("end-pos turn generation")(Catch::Benchmark::Chronometer
meter)
    {
        BitBoard board{"4k3/7r/1pn2q2/3P4/1R6/1PNQ2P1/r4R2/2B1K3 w - - 0 10"};
        meter.measure([&] { return Counter(board, 4); });
    };
}
*/
