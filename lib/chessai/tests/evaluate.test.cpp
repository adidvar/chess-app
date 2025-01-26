#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chessai/score.hpp>

/*
TEST_CASE("Evaluator tests", "[evaluate]", ) {
  SECTION("Basic tests") {
    REQUIRE(Score{}.isValid() == false);
    REQUIRE(Score::min() < Score::checkMate(10, 10));
    REQUIRE(Score::checkMate(10, 10) < Score::checkMate(0, 10));
    REQUIRE(Score::checkMate(0, 10) < Score::tie());
    REQUIRE(Score::checkMate(0, 10) < -Score::min());
    REQUIRE(Score::checkMate(100, 1000) == Score::checkMate(200, 1000));
  }
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
  SECTION("Logic tests") {
    REQUIRE(Score::GetStaticValue({"7k/8/8/8/8/8/8/1QQQQQ1K w - - 0 1"},
                                  Color::kWhite, 0) > Score::Tie());
    REQUIRE(Score::GetStaticValue({"7k/8/8/8/8/8/8/1QQQQQ1K w - - 0 1"},
                                  Color::kBlack, 0) < Score::Tie());
  }
}
*/
