#include "ttable.hpp"

#include <bitboard.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("TTable Tests", "[ttable]", ) {
  TTable table;

  BitBoard board("startpos");
  bitboard_hash_t hash = board.Hash();

  /*
  SECTION("Store/Load test") {
    table.Write(hash,Score::Tie(),;
    REQUIRE(table.GetKillerMoveCount(turn, 0) == 0);
    table.Increment(turn, 0);
    REQUIRE(table.GetKillerMoveCount(turn, 0) != 0);
    table.Clear();
    REQUIRE(table.GetKillerMoveCount(turn, 0) == 0);
  }
*/
}
