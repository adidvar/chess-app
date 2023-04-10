#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "board.hpp"
#include "bitboard.hpp"

TEMPLATE_TEST_CASE( "Board operators test", "[bitboard][board]", BitBoard, Board){
    TestType board;

    SECTION( "Test of TestColor GetFigure GetColor" ) {

        for(uint8_t i=0;i<64;i++)
        {
            for(uint8_t figure = 1; figure < 7 ; figure++){
                for(uint8_t color = 0; color < 2; color++){

                    board.Set(i,{figure,color});

                    REQUIRE(board.Test(figure,i)) ;
                    REQUIRE(board.TestColor(color,i));
                    REQUIRE(board.GetFigure(i) == figure);
                    REQUIRE(board.GetColor(i) == color);
                    REQUIRE(board.GetCell(i).color == color);
                    REQUIRE(board.GetCell(i).type == figure);
                }
            }
        }
    }

    SECTION( "Test of TestEmp" ) {
        for(int i=0;i<64;i++)
        {
            board.Set(i,{(Figure)0,(Color)0});
            REQUIRE(board.TestEmp(i));
            REQUIRE(board.GetFigure(i) == (Figure)0);
        }
    }

    SECTION( "Test of Swap" ) {
        board.Set(0,{Figure::kPawn,Color::kWhite});
        board.Set(1,{Figure::kKnight,Color::kBlack});
        board.Swap(0,1);
        REQUIRE(board.Test(Figure::kPawn,1));
        REQUIRE(board.Test(Figure::kKnight,0));
        REQUIRE(board.TestColor(Color::kBlack,0));
        REQUIRE(board.TestColor(Color::kWhite,1));
    }
}
