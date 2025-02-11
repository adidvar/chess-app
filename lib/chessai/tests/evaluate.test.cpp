#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chessai/score.hpp>

TEST_CASE("Evaluator tests", "[evaluate]", ) {
  SECTION("Basic tests") {
    REQUIRE(Score{}.isValid() == false);
    REQUIRE(Score::min() < Score::checkMate(Color::White, 10));
    REQUIRE(Score::checkMate(Color::White, 10) <
            Score::checkMate(Color::White, 0));
    REQUIRE(Score::checkMate(Color::White, 0) < Score::tie());
    REQUIRE(Score::checkMate(Color::White, 0) < -Score::min());
  }
  /*
  SECTION("Extended tests") {
    REQUIRE(Score::CheckMate(0, 10).IsCheckMate());
    REQUIRE(Score::CheckMate(100, 1000).IsCheckMate());
    REQUIRE(!Score::Tie().IsCheckMate());
    REQUIRE(Score::Max() == -Score::Min());
    REQUIRE(Score::CheckMate(10, 10).GetTurnsToCheckMate() == 0);
    REQUIRE(Score::CheckMate(0, 10).GetTurnsToCheckMate() == 10);
    REQUIRE((-Score::CheckMate(10, 10)).GetTurnsToCheckMate() == 0);
    REQUIRE((-Score::CheckMate(0, 10)).GetTurnsToCheckMate() == 10);
  }
*/
  SECTION("Logic tests") {
    REQUIRE(Score::getStaticValue({"7k/8/8/8/8/8/8/1QQQQQ1K w - - 0 1"}) >
            Score::tie());
  }
}
