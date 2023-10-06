#include "evaluate.hpp"

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Evaluate rules", "[evaluate][ai]", ) {
  using TestType = Evaluate;
  SECTION("Test of Max Min") {
    REQUIRE(TestType::Max() > TestType::Min());
    REQUIRE(TestType::Max() > TestType::Lose(0));
    REQUIRE(TestType::Max() > TestType::Win(0));
    REQUIRE(TestType::Max() > TestType::Tie());
    REQUIRE(TestType::Max() > TestType::Value(BitBoard(), Color::kWhite));
    REQUIRE(TestType::Max() > TestType::Value(BitBoard(), Color::kBlack));

    REQUIRE(TestType::Min() < TestType::Max());
    REQUIRE(TestType::Min() < TestType::Lose(0));
    REQUIRE(TestType::Min() < TestType::Win(0));
    REQUIRE(TestType::Min() < TestType::Tie());
    REQUIRE(TestType::Min() < TestType::Value(BitBoard(), Color::kWhite));
    REQUIRE(TestType::Min() < TestType::Value(BitBoard(), Color::kBlack));
  }

  SECTION("Test of Win Lose Tie") {
    REQUIRE(TestType::Win(0) > TestType::Lose(0));
    REQUIRE(TestType::Lose(0) < TestType::Win(0));

    REQUIRE(TestType::Win(0) > TestType::Tie());
    REQUIRE(TestType::Lose(0) < TestType::Tie());
  }
}
