#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <mateevaluator.hpp>

TEMPLATE_TEST_CASE( "Approximation basic rules", "[approximation][ai]", MateAppraiser){

    SECTION( "Test of Max Min" ) {
        REQUIRE(MateAppraiser::Max() > MateAppraiser::Min());
        REQUIRE(MateAppraiser::Max() > MateAppraiser::CheckMateLose());
        REQUIRE(MateAppraiser::Max() > MateAppraiser::CheckMateWin());
        REQUIRE(MateAppraiser::Max() > MateAppraiser::Tie());
        REQUIRE(MateAppraiser::Max() > MateAppraiser::Approximate(BitBoard(),Color::kWhite));
        REQUIRE(MateAppraiser::Max() > MateAppraiser::Approximate(BitBoard(),Color::kBlack));

        REQUIRE(MateAppraiser::Min() < MateAppraiser::Max());
        REQUIRE(MateAppraiser::Min() < MateAppraiser::CheckMateLose());
        REQUIRE(MateAppraiser::Min() < MateAppraiser::CheckMateWin());
        REQUIRE(MateAppraiser::Min() < MateAppraiser::Tie());
        REQUIRE(MateAppraiser::Min() < MateAppraiser::Approximate(BitBoard(),Color::kWhite));
        REQUIRE(MateAppraiser::Min() < MateAppraiser::Approximate(BitBoard(),Color::kBlack));
    }

    SECTION( "Test of Win Lose Tie" ) {
        REQUIRE(MateAppraiser::CheckMateWin() > MateAppraiser::CheckMateLose());
        REQUIRE(MateAppraiser::CheckMateLose() < MateAppraiser::CheckMateWin());

        REQUIRE(MateAppraiser::CheckMateWin() > MateAppraiser::CheckMateWin().Process());
        REQUIRE(MateAppraiser::CheckMateLose().Process() < MateAppraiser::CheckMateWin().Process());

        REQUIRE(MateAppraiser::CheckMateWin() > MateAppraiser::Tie());
        REQUIRE(MateAppraiser::CheckMateLose() < MateAppraiser::Tie());
    }
}
