#include "ttable.hpp"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("TTable Tests", "[ttable]", ) {
  TTable table;

  BitBoard board("startpos");
  bitboard_hash_t hash = board.Hash();
  Score a{-100};
  Score b{100};
  Score value{0};
  auto turn = Turn::FromChessFormat("e2e4");
  uint8_t depth = 7;

  SECTION("Store/Load test") {
    table.Write(hash, a, b, value, turn, depth);
    bool find = false;
    const auto *elem = table.Search(hash, find);
    REQUIRE(find);
    REQUIRE(elem->depth == depth);
    REQUIRE(elem->pv == turn);
    REQUIRE(elem->type == TTableItem::PV);
    REQUIRE(elem->value == value);
  }
  SECTION("Node type test") {
    bool find = false;
    table.Write(hash, a, b, value, turn, depth);
    auto elem = table.Search(hash, find);
    REQUIRE(find);
    REQUIRE(elem->type == TTableItem::PV);
    table.Write(hash, a, value, b, turn, depth);
    elem = table.Search(hash, find);
    REQUIRE(find);
    REQUIRE(elem->type == TTableItem::FailHigh);
    table.Write(hash, value, b, a, turn, depth);
    elem = table.Search(hash, find);
    REQUIRE(find);
    REQUIRE(elem->type == TTableItem::FailLow);
  }
  SECTION("Clear test") {
    bool find = false;
    table.Write(hash, a, b, value, turn, depth);
    table.Clear();
    auto elem = table.Search(hash, find);
    REQUIRE_FALSE(find);
  }
  SECTION("Garbage collection test") {
    bool find = false;
    table.Write(hash, a, b, value, turn, depth);
    table.Search(hash, find);
    REQUIRE(find);
    table.SetGarbageFlag();
    table.Search(hash, find);
    REQUIRE(find);
    table.ClearGarbage();
    table.Search(hash, find);
    REQUIRE(find);
    table.SetGarbageFlag();
    table.ClearGarbage();
    table.Search(hash, find);
    REQUIRE_FALSE(find);
  }
}

TEST_CASE("TTable Benchmarks", "[ttable]", ) {
  TTable table;

  BitBoard board("startpos");
  bitboard_hash_t hash = board.Hash();
  Score value{0};
  auto turn = Turn::FromChessFormat("e2e4");
  uint8_t depth = 7;

  BENCHMARK("Load benchmark") {
    bool find = false;
    auto e = table.Search(rand(), find);
    return e;
  };
  BENCHMARK("Store benchmark") {
    bool find = false;
    table.Write(rand(), value, value, value, {}, depth);
    return find;
  };
}
