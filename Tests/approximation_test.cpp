#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <mateevaluator.hpp>
#include <position_rating.hpp>

TEMPLATE_TEST_CASE( "Approximation basic rules", "[approximation][ai]", MateAppraiser){

    SECTION( "Test of Max Min" ) {
        REQUIRE(TestType::Max() > TestType::Min());
        REQUIRE(TestType::Max() > TestType::CheckMateLose());
        REQUIRE(TestType::Max() > TestType::CheckMateWin());
        REQUIRE(TestType::Max() > TestType::Tie());
        REQUIRE(TestType::Max() > TestType::Approximate(BitBoard(),Color::kWhite));
        REQUIRE(TestType::Max() > TestType::Approximate(BitBoard(),Color::kBlack));

        REQUIRE(TestType::Min() < TestType::Max());
        REQUIRE(TestType::Min() < TestType::CheckMateLose());
        REQUIRE(TestType::Min() < TestType::CheckMateWin());
        REQUIRE(TestType::Min() < TestType::Tie());
        REQUIRE(TestType::Min() < TestType::Approximate(BitBoard(),Color::kWhite));
        REQUIRE(TestType::Min() < TestType::Approximate(BitBoard(),Color::kBlack));
    }

    SECTION( "Test of Win Lose Tie" ) {
        REQUIRE(TestType::CheckMateWin() > TestType::CheckMateLose());
        REQUIRE(TestType::CheckMateLose() < TestType::CheckMateWin());

        REQUIRE(TestType::CheckMateWin() > TestType::Tie());
        REQUIRE(TestType::CheckMateLose() < TestType::Tie());
    }
}
