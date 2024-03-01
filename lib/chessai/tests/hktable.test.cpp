#include "hktable.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("HKTable Tests", "[hktable]", ) {
  HKTable table;
  auto turn = Turn::FromChessFormat("e2e4");
  SECTION("Killer counters") {
    REQUIRE(table.GetKillerMoveCount(turn, 0) == 0);
    table.Increment(turn, 0);
    REQUIRE(table.GetKillerMoveCount(turn, 0) != 0);
    table.Clear();
    REQUIRE(table.GetKillerMoveCount(turn, 0) == 0);
  }
  SECTION("History counters") {
    REQUIRE(table.GetHistoryCount(turn) == 0);
    table.Increment(turn, 0);
    REQUIRE(table.GetHistoryCount(turn) != 0);
    table.Clear();
    REQUIRE(table.GetHistoryCount(turn) == 0);
  }
}
