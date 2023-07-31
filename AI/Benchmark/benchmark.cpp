#include <alphabeta.hpp>
#include <chrono>
#include <computer.hpp>
#include <iostream>
#include <match.hpp>
#include <minmax.hpp>
#include <statistics.hpp>

#include "evaluate.hpp"
#include "qsearch.hpp"

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

    // auto board =
    // BitBoard("r3k3/pppppppp/1nb2b1r/3q2n1/2Q1P3/2BN1NB1/PPPP1PPP/R3K2R w KQq
    // - 0 1");
    auto tbegin = std::chrono::high_resolution_clock::now();
    Statistics stat;
    AlphaBeta<Evaluate> ab(Color::kWhite, stat);
    // auto board = BitBoard(
    //     "r3kb1r/ppqpp1pp/2n2pn1/2p1P2Q/3b4/2BN1NB1/PPPP1PPP/R3K2R w KQkq - 0
    //     " "1");
    //  auto board =
    /*
    BitBoard board(
        "4k3/pppppppp/1bq4r/4n3/rNBN1Qn1/4B2R/PPPPPPPP/4K3 w - - 0 1");
*/
    auto board = BitBoard();
    for (size_t d = 1; d <= 7; d++) {
      stat.Clear();
      auto begin = std::chrono::high_resolution_clock::now();
      auto result = ab.GetValue(board, d);
      auto delay = std::chrono::high_resolution_clock::now() - begin;

      std::cout << "Nodes: " << stat.GetMainNode() << std::endl;
      std::cout << "ENodes: " << stat.GetExtraNode() << std::endl;
      //  std::cout << "Fill: " << table.Fill() << std::endl;
      std::cout << "Time: "
                << std::chrono::duration_cast<
                       std::chrono::duration<float, std::ratio<1, 1>>>(delay)
                       .count()
                << std::endl;
      std::cout << "Depth: " << d << std::endl;
      std::cout << "Result: " << result.ToString() << std::endl;
      std::cout << "------------------------------------" << std::endl;
    }
    auto delay = std::chrono::high_resolution_clock::now() - tbegin;
    std::cout << "Time: "
              << std::chrono::duration_cast<
                     std::chrono::duration<float, std::ratio<1, 1>>>(delay)
                     .count()
              << std::endl;

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
