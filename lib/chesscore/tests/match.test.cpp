#include <catch2/catch_test_macros.hpp>
#include <match.hpp>

TEST_CASE("Match from UCI string", "[match]") {
  Match match;
  REQUIRE_NOTHROW(match.LoadFromUCIString("startpos e4 e5"));
  REQUIRE(match.GetStartBoard() == BitBoard());
  REQUIRE(match.GetBoard() ==
          BitBoard(
              "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"));

  REQUIRE_NOTHROW(match.LoadFromUCIString(
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 e4 e5"));
  REQUIRE(match.GetStartBoard() == BitBoard());
  REQUIRE(match.GetBoard() ==
          BitBoard(
              "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"));
}
