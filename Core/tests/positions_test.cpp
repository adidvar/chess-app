#include <catch2/catch_test_macros.hpp>
#include "position.hpp"

TEST_CASE( "Positions operators", "[positions]" )
{
   REQUIRE(Position(0,0).Valid() == true);
   REQUIRE(Position(7,7).Valid() == true);
   REQUIRE(Position(0).Valid() == true) ;
   REQUIRE(Position(63).Valid() == true);
   REQUIRE(Position(64).Valid() == false);
   REQUIRE(Position(0).ToString() == "a8");
   REQUIRE(Position(56).ToString() == "a1");
   REQUIRE(Position(7).ToString() == "h8");
   REQUIRE(Position(63).ToString() == "h1");
   REQUIRE("a8" == Position::FromString("a8").ToString());
   REQUIRE("a1" == Position::FromString("a1").ToString());
   REQUIRE("h8" == Position::FromString("h8").ToString());
   REQUIRE("h1" == Position::FromString("h1").ToString());
   REQUIRE(!Position::FromString("m8").Valid());
   REQUIRE(!Position::FromString("a9").Valid());
   REQUIRE(!Position::FromString("h0").Valid());
   REQUIRE(!Position::FromString("z1").Valid());
}
