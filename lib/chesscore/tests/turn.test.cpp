#include <catch2/catch_test_macros.hpp>
#include <figures.hpp>

TEST_CASE("Turn from string")
{
    REQUIRE(Turn("a1a8").toString() == "a1a8");
    REQUIRE(Turn("a2h8").toString() == "a2h8");
    REQUIRE(Turn("a3a5").toString() == "a3a5");
    REQUIRE(Turn("a6a8q").toString() == "a6a8q");
    REQUIRE(!Turn("a1a9").isValid());
    REQUIRE(!Turn("a1a9h").isValid());
    REQUIRE(!Turn("a1a8h").isValid());
    REQUIRE(!Turn("v1a2").isValid());
    REQUIRE(!Turn("a1a9m").isValid());
    REQUIRE(!Turn("a1a2m").isValid());
}

TEST_CASE("Turn default constructor")
{
    Turn turn;

    REQUIRE(turn.isValid() == false);
}

TEST_CASE("Turn parameterized constructors")
{
    Position from(0, 0);
    Position to(0, 1);
    Figure figure = Figure::Pawn;

    SECTION("Constructor with 'from' and 'to' positions")
    {
        Turn turn(from, to);

        REQUIRE(turn.from() == from);
        REQUIRE(turn.to() == to);
        REQUIRE(turn.isValid() == true);
    }

    SECTION("Constructor with 'from', 'to' and 'figure'")
    {
        Turn turn(from, to, figure);

        REQUIRE(turn.from() == from);
        REQUIRE(turn.to() == to);
        REQUIRE(turn.isValid() == true);
        REQUIRE(turn.isTrasformation() == true); // Assuming no transformation occurs here
    }
}

TEST_CASE("Turn string constructor")
{
    SECTION("Valid chess notation string")
    {
        Turn turn("e2e4");

        REQUIRE(turn.from() == Position("e2")); // 'e2' -> (4, 1)
        REQUIRE(turn.to() == Position("e4"));   // 'e4' -> (4, 3)
        REQUIRE(turn.isValid() == true);
    }

    SECTION("Invalid chess notation string")
    {
        Turn turn("invalid");

        REQUIRE(turn.isValid() == false);
    }
}

TEST_CASE("Turn comparison operators")
{
    Turn turn1(Position(0, 0), Position(0, 1));
    Turn turn2(Position(0, 0), Position(0, 1));
    Turn turn3(Position(0, 1), Position(0, 2));

    SECTION("Equality operator")
    {
        REQUIRE(turn1 == turn2);
        REQUIRE(turn1 != turn3);
    }
}

TEST_CASE("Turn copy and assignment")
{
    Turn original(Position(0, 0), Position(0, 1), Figure::Pawn);

    SECTION("Copy constructor")
    {
        Turn copy(original);

        REQUIRE(copy == original);
    }

    SECTION("Assignment operator")
    {
        Turn assigned;
        assigned = original;

        REQUIRE(assigned == original);
    }
}
