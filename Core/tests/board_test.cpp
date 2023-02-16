#include <catch2/catch_test_macros.hpp>
#include "board.h"

class Board_Test : public Board
{
public:
    void Testing()
    {
        SECTION( "set and get test" ) {
            for(int i=0;i<64;i++)
            {
                for(size_t fig = 0; fig < 7 ; fig++){
                    for(size_t color = 0; color < 2; color++){

                        Set(i,{(Figure)fig,(Color)color});

                        REQUIRE(Test((Figure)fig,i)) ;
                        REQUIRE(TestColor((Color)color,i)) ;
                        REQUIRE(GetFigure(i) == (Figure)fig);
                        REQUIRE(GetColor(i) == (Color)color);

                    }
                }
            }
        }

        SECTION( "swap test" ) {
            Set(0,{Figure::kPawn,Color::kWhite});
            Set(1,{Figure::kKnight,Color::kBlack});
            Swap(0,1);
            REQUIRE(Test(Figure::kPawn,1));
            REQUIRE(Test(Figure::kKnight,0));
            REQUIRE(TestColor(Color::kBlack,0));
            REQUIRE(TestColor(Color::kWhite,1));
        }
    }
};

TEST_CASE( "ChessBoard operators", "[chessboard]" ) {
    Board_Test test;
    test.Testing();
}
