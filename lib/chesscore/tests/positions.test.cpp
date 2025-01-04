#include <catch2/catch_test_macros.hpp>
#include <position.hpp>

TEST_CASE("Position from string")
{
    REQUIRE(Position(0, 0).isValid() == true);
    REQUIRE(Position(7, 7).isValid() == true);
    REQUIRE(Position(0).isValid() == true);
    REQUIRE(Position(63).isValid() == true);
    REQUIRE(Position(64).isValid() == false);
    REQUIRE(Position(0).toString() == "a8");
    REQUIRE(Position(56).toString() == "a1");
    REQUIRE(Position(7).toString() == "h8");
    REQUIRE(Position(63).toString() == "h1");
    REQUIRE("a8" == Position("a8").toString());
    REQUIRE("a1" == Position("a1").toString());
    REQUIRE("h8" == Position("h8").toString());
    REQUIRE("h1" == Position("h1").toString());
    REQUIRE(!Position("m8").isValid());
    REQUIRE(!Position("a9").isValid());
    REQUIRE(!Position("h0").isValid());
    REQUIRE(!Position("z1").isValid());
}

TEST_CASE("Position default constructor")
{
    Position pos;

    REQUIRE(pos.isValid() == false);
    REQUIRE(pos.toString() == "--");
}

TEST_CASE("Position parameterized constructors")
{
    SECTION("Valid x and y coordinates")
    {
        Position pos(3, 4);

        REQUIRE(pos.isValid() == true);
        REQUIRE(pos.x() == 3);
        REQUIRE(pos.y() == 4);
        REQUIRE(pos.index() == 35); // 4 * 8 + 3
    }

    SECTION("Valid index")
    {
        Position pos(27); // Index corresponds to (3, 3)

        REQUIRE(pos.isValid() == true);
        REQUIRE(pos.x() == 3);
        REQUIRE(pos.y() == 3);
    }

    SECTION("Invalid index")
    {
        Position pos(64); // Invalid index

        REQUIRE(pos.isValid() == false);
        REQUIRE(pos.toString() == "--");
    }

    SECTION("String representation constructor (valid)")
    {
        Position pos("e2");

        REQUIRE(pos.isValid() == true);
        REQUIRE(pos.x() == 4); // 'e' -> 4
        REQUIRE(pos.y() == 6); // '2' -> 8 - 2 = 6
        REQUIRE(pos.toString() == "e2");
    }

    SECTION("String representation constructor (invalid)")
    {
        Position pos("z9");

        REQUIRE(pos.isValid() == false);
        REQUIRE(pos.toString() == "--");
    }
}

TEST_CASE("Position rotation")
{
    SECTION("Valid position rotation")
    {
        Position pos(0, 0);
        Position rotated = pos.rotate();

        REQUIRE(rotated.isValid() == true);
        REQUIRE(rotated.x() == 7);
        REQUIRE(rotated.y() == 7);
        REQUIRE(rotated.index() == 63);
    }
}

TEST_CASE("Position toString")
{
    SECTION("Valid position")
    {
        Position pos(4, 3);

        REQUIRE(pos.toString() == "e5");
    }

    SECTION("Invalid position")
    {
        Position invalid;

        REQUIRE(invalid.toString() == "--");
    }
}

TEST_CASE("Position comparison operators")
{
    Position pos1(3, 4);
    Position pos2(3, 4);
    Position pos3(2, 4);

    SECTION("Equality operator")
    {
        REQUIRE(pos1 == pos2);
        REQUIRE(pos1 != pos3);
    }

    SECTION("Less-than operator")
    {
        REQUIRE(pos3 < pos1);
        REQUIRE(!(pos1 < pos2));
    }
}

TEST_CASE("Edge cases")
{
    SECTION("Position on the edge of the board")
    {
        Position pos(7, 7);

        REQUIRE(pos.isValid() == true);
        REQUIRE(pos.x() == 7);
        REQUIRE(pos.y() == 7);
        REQUIRE(pos.index() == 63);
    }
}
