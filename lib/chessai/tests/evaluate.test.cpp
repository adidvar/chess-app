#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "score.hpp"

TEST_CASE("Evaluate Tests", "[evaluate]", ) {
  SECTION("Basic tests") {
    REQUIRE(Score{}.IsValid() == false);
    REQUIRE(Score::Min() < Score::CheckMate(10, 10));
    REQUIRE(Score::CheckMate(10, 10) < Score::CheckMate(0, 10));
    REQUIRE(Score::CheckMate(0, 10) < Score::Tie());
    REQUIRE(Score::CheckMate(0, 10) < -Score::Min());
    REQUIRE(Score::CheckMate(100, 1000) == Score::CheckMate(200, 1000));
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
  SECTION("Evaluator tests") {
    REQUIRE(Score::GetStaticValue({"7k/8/8/8/8/8/8/1QQQQQ1K w - - 0 1"},
                                  Color::kWhite, GameStage{}) > Score::Tie());
    REQUIRE(Score::GetStaticValue({"7k/8/8/8/8/8/8/1QQQQQ1K w - - 0 1"},
                                  Color::kBlack, GameStage{}) < Score::Tie());
  }
}
