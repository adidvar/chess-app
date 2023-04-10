#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include "board.hpp"
#include "bitboard.hpp"

TEMPLATE_TEST_CASE( "Board operators test", "[bitboard][board][fen]", BitBoard, Board){

    SECTION( "Test of TestColor GetFigure GetColor" ) {
        TestType board;
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
        TestType board;
        for(int i=0;i<64;i++)
        {
            board.Set(i,{Figure::kEmpty,Color::kWhite});
            REQUIRE(board.TestEmp(i));
            REQUIRE(board.GetFigure(i) == (Figure)0);
        }
    }

    SECTION( "Test of Swap" ) {
        TestType board;
        for(size_t i = 0 ; i < 64 ; i++){
            for(size_t j = 0 ; j < 64 ; j++){
                if(i==j)
                    continue;

                board.Set(i,{Figure::kPawn,Color::kWhite});
                board.Set(j,{Figure::kKnight,Color::kBlack});
                board.Swap(i,j);
                REQUIRE(board.Test(Figure::kPawn,j));
                REQUIRE(board.Test(Figure::kKnight,i));
                REQUIRE(board.TestColor(Color::kBlack,i));
                REQUIRE(board.TestColor(Color::kWhite,j));
            }
        }
    }
    SECTION( "Test of SkipMove CurrentColor OpponentColor" ) {
        TestType board;
        auto copy = board;
        copy.SkipMove();
        REQUIRE(board.CurrentColor() == copy.OpponentColor());
        REQUIRE(copy.CurrentColor() == board.OpponentColor());
    }
    SECTION( "Test of ExecuteTurn GenerateTurns TestTurn" ) {
        TestType board;
        auto turns = board.GenerateTurns();
        auto subboards = board.GenerateSubBoards();
        REQUIRE(turns.size() == subboards.size());

        for(size_t i = 0 ; i < turns.size(); i++){
            TestType sub = board;
            sub.ExecuteTurn(turns[i]);
            REQUIRE(sub.Fen() == subboards[i].Fen());
        }
    }
    SECTION( "Test of el passant" ) {
        TestType board;
        REQUIRE(board.ExecuteTurn(Turn(Position(51),Position(35))));
        REQUIRE(board.LastPawnMove() == Position(43));
        REQUIRE(board.ExecuteTurn(Turn(Position(9),Position(17))));
        REQUIRE(board.LastPawnMove() == Position());
        board = TestType();
        board.SkipMove();
        REQUIRE(board.ExecuteTurn(Turn(Position(9),Position(25))));
        REQUIRE(board.LastPawnMove() == Position(17));
        REQUIRE(board.ExecuteTurn(Turn(Position(57),Position(42))));
        REQUIRE(board.LastPawnMove() == Position());
    }
    SECTION( "Test of castling" ) {
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(60),Position(62))));
            REQUIRE(!board.RookingFlags().white_ooo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(60),Position(58))));
            REQUIRE(!board.RookingFlags().white_ooo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(60),Position(52))));
            REQUIRE(!board.RookingFlags().white_ooo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(56),Position(48))));
            REQUIRE(!board.RookingFlags().white_ooo);
            REQUIRE(board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(63),Position(55))));
            REQUIRE(board.RookingFlags().white_ooo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(56),Position(0))));
            REQUIRE(!board.RookingFlags().white_ooo);
            REQUIRE(!board.RookingFlags().black_ooo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R w KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(63),Position(7))));
            REQUIRE(!board.RookingFlags().black_oo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(4),Position(6))));
            REQUIRE(!board.RookingFlags().black_ooo);
            REQUIRE(!board.RookingFlags().black_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(4),Position(2))));
            REQUIRE(!board.RookingFlags().black_ooo);
            REQUIRE(!board.RookingFlags().black_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(4),Position(12))));
            REQUIRE(!board.RookingFlags().black_ooo);
            REQUIRE(!board.RookingFlags().black_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(0),Position(8))));
            REQUIRE(!board.RookingFlags().black_ooo);
            REQUIRE(board.RookingFlags().black_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(7),Position(15))));
            REQUIRE(board.RookingFlags().black_ooo);
            REQUIRE(!board.RookingFlags().black_oo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(0),Position(56))));
            REQUIRE(!board.RookingFlags().black_ooo);
            REQUIRE(!board.RookingFlags().white_ooo);
        }
        {
            TestType board("r3k2r/1ppp1pp1/8/8/8/8/1PPP1PP1/R3K2R b KQkq a6 0 1");
            REQUIRE(board.ExecuteTurn(Turn(Position(7),Position(63))));
            REQUIRE(!board.RookingFlags().black_oo);
            REQUIRE(!board.RookingFlags().white_oo);
        }
    }
    SECTION( "Test of MateTest End Checkmate WhiteWin BlackWin Tie" ) {
        {
            TestType board;
            REQUIRE(!board.MateTest());
            REQUIRE(!board.End());
            REQUIRE(!board.Checkmate());
            REQUIRE(!board.WhiteWin());
            REQUIRE(!board.BlackWin());
            REQUIRE(!board.Tie());
        }
        {
            TestType board("rnb1kbnr/pppppppp/8/4q3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
            REQUIRE(board.MateTest());
            REQUIRE(!board.End());
            REQUIRE(!board.Checkmate());
            REQUIRE(!board.WhiteWin());
            REQUIRE(!board.BlackWin());
            REQUIRE(!board.Tie());
        }
        {
            TestType board("rnb1k2r/pppppppp/6n1/4bq2/7K/8/PPPP1PPP/RNBQ1BNR w KQkq - 0 1");
            REQUIRE(board.MateTest());
            REQUIRE(board.End());
            REQUIRE(board.Checkmate());
            REQUIRE(!board.WhiteWin());
            REQUIRE(board.BlackWin());
            REQUIRE(!board.Tie());
        }
        {
            TestType board("rnb4r/ppppp3/6N1/4BQ2/7k/8/PPPP1PPP/RNBK3R b KQkq - 0 1");
            REQUIRE(board.MateTest());
            REQUIRE(board.End());
            REQUIRE(board.Checkmate());
            REQUIRE(board.WhiteWin());
            REQUIRE(!board.BlackWin());
            REQUIRE(!board.Tie());
        }
        {
            TestType board("k7/8/1Q6/2K5/8/8/8/8 b - - 0 1");
            REQUIRE(!board.MateTest());
            REQUIRE(board.End());
            REQUIRE(!board.Checkmate());
            REQUIRE(board.WhiteWin());
            REQUIRE(!board.BlackWin());
            REQUIRE(board.Tie());
        }
        {
            TestType board("K7/8/1q6/2k5/8/8/8/8 w - - 0 1");
            REQUIRE(!board.MateTest());
            REQUIRE(board.End());
            REQUIRE(!board.Checkmate());
            REQUIRE(!board.WhiteWin());
            REQUIRE(board.BlackWin());
            REQUIRE(board.Tie());
        }
    }
}


/*
    size_t TurnCounter() const noexcept;
    size_t PassiveTurnCounter() const noexcept;

    bool MateTest() const;
    bool End() const;
    bool Checkmate() const;
    bool WhiteWin() const;
    bool BlackWin() const;
    bool Tie() const;
    */
