#include <catch2/catch_test_macros.hpp>
#include "positions.h"

TEST_CASE( "Positions operators", "[positions]" )
{
   REQUIRE(Position(0,0) == true);
   REQUIRE(Position(7,7) == true);
   REQUIRE(Position(0) == true) ;
   REQUIRE(Position(63) == true);
   REQUIRE(Position(64) == false);
}
