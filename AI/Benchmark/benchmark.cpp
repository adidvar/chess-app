#include <iostream>
#include <minmax.hpp>
#include <position_rating.hpp>
#include <statistics.hpp>
#include <mateevaluator.hpp>
#include <chrono>
#include <alphabeta.hpp>

template <typename T>
std::string pt(T t){
    return  std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(t).count()) + " ms";
}

template<typename T>
void Bench(BitBoard board , bitboard_t depth){
    using namespace std;

    cout << "-------------------BENCH------------------------------" << endl;
    cout << depth << "<-->";
    cout << board.Fen() << endl;
    cout << "------------------------------------------------------" << endl;

    Statistics stat;
    T mm(Color::kWhite,stat);

    auto res = mm.Evaluate(board,depth);

    cout << "         :" << pt(stat.GetAllTime()) << ":" << endl;
    cout << "G: " << pt(stat.GetGenerationTime()) << ":       A: " << pt(stat.GetApproximationTime()) << ":   O: "
         << pt(stat.GetAllTime() - stat.GetApproximationTime() - stat.GetGenerationTime())  << endl;

    float gp = (float)stat.GetGenerationTime().count()/stat.GetAllTime().count() , ap = (float)stat.GetApproximationTime().count()/stat.GetAllTime().count(), op = 1-gp-ap;



    cout << "------------------------------------------------------" << endl;
    cout << "G: " << 100*gp << ":      A: " << 100*ap << ":      O: "
         << 100*op << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "N: " << stat.GetNodesCount() << ":      AN: " << stat.GetApproximationCount() << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "R: "<< res.ToString(depth) << endl;
    cout << "------------------------------------------------------" << endl;

    cout << endl;
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
    auto b = BitBoard();
    //b.SkipMove();
    //auto b = BitBoard("8/3K4/6q1/8/q7/4k3/8/8 w - - 0 1");
    auto d = 5;
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
