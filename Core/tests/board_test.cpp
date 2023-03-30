#include <catch2/catch_test_macros.hpp>
#include "board.h"
#include "bitboard.h"

template<class T>
class Board_Test : public T
{
public:
    void Testing()
    {
        SECTION( "Test TestColor GetFigure GetColor" ) {
            for(int i=0;i<64;i++)
            {
                for(size_t fig = 1; fig < 3 ; fig++){
                    for(size_t color = 0; color < 2; color++){

                        T::Set(i,{(Figure)fig,(Color)color});

                        REQUIRE(T::Test((Figure)fig,i)) ;
                        REQUIRE(T::TestColor((Color)color,i)) ;
                        REQUIRE(T::GetFigure(i) == (Figure)fig);
                        REQUIRE(T::GetColor(i) == (Color)color);
                        REQUIRE(T::GetCell(i).color == (Color)color);
                        REQUIRE(T::GetCell(i).type == (Figure)fig);

                    }
                }
            }
        }
        SECTION( "TestEmp" ) {
            for(int i=0;i<64;i++)
            {
                T::Set(i,{(Figure)0,(Color)0});
                REQUIRE(T::TestEmp(i));
                REQUIRE(T::GetFigure(i) == (Figure)0);
            }
        }

        SECTION( "Swap" ) {
            T::Set(0,{Figure::kPawn,Color::kWhite});
            T::Set(1,{Figure::kKnight,Color::kBlack});
            T::Swap(0,1);
            REQUIRE(T::Test(Figure::kPawn,1));
            REQUIRE(T::Test(Figure::kKnight,0));
            REQUIRE(T::TestColor(Color::kBlack,0));
            REQUIRE(T::TestColor(Color::kWhite,1));
        }
    }
};

TEST_CASE( "ChessBoard operators", "[chessboard]" ) {
    Board_Test<Board> test;
    test.Testing();
}

TEST_CASE( "BitBoard operators", "[bitboard]" ) {
    Board_Test<BitBoard> test;
    test.Testing();
}
