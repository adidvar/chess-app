#include <catch2/catch_test_macros.hpp>
#include <turn.hpp>

TEST_CASE( "Turn testing", "[turn]" )
{
    REQUIRE(Turn::FromChessFormat("a1a8").ToChessFormat() == "a1a8");
    REQUIRE(Turn::FromChessFormat("a2h8").ToChessFormat() == "a2h8");
    REQUIRE(Turn::FromChessFormat("a3a5").ToChessFormat() == "a3a5");
    REQUIRE(Turn::FromChessFormat("a6a8q").ToChessFormat() == "a6a8q");
    REQUIRE(!Turn::FromChessFormat("a1a9").Valid());
    REQUIRE(!Turn::FromChessFormat("a1a9h").Valid());
    REQUIRE(!Turn::FromChessFormat("a1a8h").Valid());
    REQUIRE(!Turn::FromChessFormat("v1a2").Valid());
    REQUIRE(!Turn::FromChessFormat("a1a9m").Valid());
    REQUIRE(!Turn::FromChessFormat("a1a2m").Valid());
}
