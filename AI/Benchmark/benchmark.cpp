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

   Match match;
   Computer computer(match,Color::kWhite);
   computer.Start();
   computer.Wait();

   std::vector<std::pair<Turn,MainAppraiser>> turns;
   computer.LoadTurnsMarks(turns);

   /*
   std::vector<MainAppraiser> turnse;
   for(auto turn : match.GetBoard().GenerateTurns(Color::kWhite)){
       auto board = match.GetBoard();
       board.ExecuteTurn(turn);
       NoStatistics stat;
       AlphaBeta<MainAppraiser> evaluator(Color::kWhite,stat);
       auto value = evaluator.Evaluate(board,5);
       turnse.push_back(value);
   }
   */


   for(auto pair : turns) {
       std::cout << pair.first.ToChessFormat() << "<--->" << pair.second.ToString(0) << "::::" << std::endl;

   }


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
