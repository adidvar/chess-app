#include "gtest/gtest.h"
#include "positions.h"

TEST(Core,position)
{
   ASSERT_EQ(Position(0,0) ,0) << "Position (0,0) is not zero";
   ASSERT_EQ(Position(7,7) ,63) << "Position (8,8) is not 64";
   ASSERT_TRUE(PositionNormal(0)) <<" 0 is normal position";
   ASSERT_TRUE(PositionNormal(63)) <<" 63 is normal position";
   ASSERT_FALSE(PositionNormal(64)) <<" 64 isn`t normal position";
}
