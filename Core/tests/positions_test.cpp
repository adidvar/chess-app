#include <catch2/catch_test_macros.hpp>
#include "position.hpp"

TEST_CASE( "Positions operators", "[positions]" )
{
   REQUIRE(Position(0,0).Valid() == true);
   REQUIRE(Position(7,7).Valid() == true);
   REQUIRE(Position(0).Valid() == true) ;
   REQUIRE(Position(63).Valid() == true);
   REQUIRE(Position(64).Valid() == false);
}
