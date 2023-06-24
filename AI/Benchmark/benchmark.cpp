#include <iostream>
#include <minmax.hpp>
#include <position_rating.hpp>
#include <statistics.hpp>
#include <mateevaluator.hpp>
#include <chrono>
#include <alphabeta.hpp>
#include <match.hpp>
#include <computer.hpp>

template <typename T>
std::string pt(T t){
    return  std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(t).count()) + " ms";
}

Position GenPos(){
    return Position(rand()%64);
}

BitBoard GenBoard(){
    BitBoard board("8/8/8/8/8/8/8/8 w - - 0 1");

    board.Set(GenPos(),{Figure::kKing,Color::kWhite});
    board.Set(GenPos(),{Figure::kKing,Color::kBlack});
    board.Set(GenPos(),{Figure::kQueen,Color::kBlack});

    return board;
}

int main(){
    //auto b = BitBoard();
    //b.SkipMove();
    //auto b = BitBoard("8/3K4/6q1/8/q7/4k3/8/8 w - - 0 1");
    //auto d = 5;

    //auto board = BitBoard("r3k3/pppppppp/1nb2b1r/3q2n1/2Q1P3/2BN1NB1/PPPP1PPP/R3K2R w KQq - 0 1");
    Statistics stat;
    TransPositionTable table;
    AlphaBeta<MainAppraiser> ab(Color::kWhite,stat,table);
    auto board = BitBoard();
    /*
    {
        auto begin = std::chrono::high_resolution_clock::now();
        auto result = ab.GetValue(board,5);
        auto delay = std::chrono::high_resolution_clock::now() - begin;

        std::cout << "Nodes: " << stat.GetGeneration() << std::endl;
        std::cout << "Approximation: " << stat.GetApproximation() << std::endl;
        std::cout << "Extra: " << stat.GetExtraNode() << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float,std::ratio<1,1>>>(delay).count() << std::endl;
        std::cout << "Result: " << result.ToString(7) << std::endl;
    }
    */
    {
        stat.Clear();
        auto begin = std::chrono::high_resolution_clock::now();
        auto result = ab.GetValue(board,6);
        auto delay = std::chrono::high_resolution_clock::now() - begin;

        std::cout << "Nodes: " << stat.GetGeneration() << std::endl;
        std::cout << "Approximation: " << stat.GetApproximation() << std::endl;
        std::cout << "Extra: " << stat.GetExtraNode() << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float,std::ratio<1,1>>>(delay).count() << std::endl;
        std::cout << "Result: " << result.ToString(7) << std::endl;
    }
    {
        stat.Clear();
        auto begin = std::chrono::high_resolution_clock::now();
        auto result = ab.GetValue(board,7);
        auto delay = std::chrono::high_resolution_clock::now() - begin;

        std::cout << "Nodes: " << stat.GetGeneration() << std::endl;
        std::cout << "Approximation: " << stat.GetApproximation() << std::endl;
        std::cout << "Extra: " << stat.GetExtraNode() << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float,std::ratio<1,1>>>(delay).count() << std::endl;
        std::cout << "Result: " << result.ToString(7) << std::endl;
    }

    /*
    BitBoard board("8/8/2q2P1N/1P6/8/p1Q2B2/8/8 w - - 0 1");
    auto boards = board.GenerateSubBoards(board.CurrentColor());
    auto turns = board.GenerateTurns(board.CurrentColor());
    ReOrder(board,turns,boards,false);

    for(auto turn : turns)
        std::cout << turn.ToChessFormat() << std::endl;

   */
/*
    for(;;){
       // std::string string;
        //std::getline(std::cin, string);

        auto board = GenBoard();

        for(;;){
            NoStatistics stat;
            auto stat1 = MinMax<ValueAppraiser,NoStatistics>::Evaluate(board,Color::kWhite, 6, stat);
            auto stat2 =  AlphaBeta<ValueAppraiser,NoStatistics>::Evaluate(board,Color::kWhite, 6, stat);
            if(stat1 != stat2){
                std::cout << board.Fen() << " " << stat1.ToString() << "  " << stat2.ToString() << std::endl;
            }
            auto turns = board.GenerateSubBoards();
            board = GenBoard();
            std::cout << "L" << std::endl;
        }
    }
    */
}
