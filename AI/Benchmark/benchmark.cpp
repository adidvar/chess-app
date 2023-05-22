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
void Bench(BitBoard board , uint64_t depth){
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
    cout << "R: "<< res.ToString() << endl;
    cout << "------------------------------------------------------" << endl;

    cout << endl;

}

int main(){
    auto b = BitBoard("8/3K4/6q1/8/q7/4k3/8/8 w - - 0 1");
    auto d = 6;
    Bench<MinMax<MateAppraiser,Statistics>>(b,d);
    Bench<AlphaBeta<MateAppraiser,Statistics>>(b,d);
}
