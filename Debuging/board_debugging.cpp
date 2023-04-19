#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
#include <computer.hpp>


template<class Board>
static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    size_t counter = 0;

    for(auto board: board.GenerateSubBoards())
        counter += MovesCounter(board,depth-1);

    return counter;
}

std::vector<std::vector<BitBoard>> hash(9);

static size_t MovesCounterFast(BitBoard board,size_t depth){
    if(!depth)
        return 1;
    size_t counter = 0;

    board.GenerateSubBoards(hash[depth]);
    for(auto board: hash[depth])
        counter += MovesCounter(board,depth-1);

    return counter;
}

void SeeBoard(BitBoard board , size_t depth)
{
    using namespace std;
    auto begin = std::chrono::high_resolution_clock::now();
    for(auto board : board.GenerateSubBoards()){
        cout << board.Fen() << ": " << MovesCounter(board,depth-1) << endl;
    }
    auto end = std::chrono::high_resolution_clock::now() - begin;
    std::cout << "---------------------------------" << std::endl;
    std::cout << "   BitBoard:" << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << "   <--->  ";
    cout << MovesCounter(board,depth) << endl;
}


float BenchMarkOnce(){

    using namespace std::chrono;
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<Board>(Board(),4);
    auto end = std::chrono::high_resolution_clock::now() - begin;
    auto last_dur = end;

    begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<BitBoard>(BitBoard(),4);
    end = std::chrono::high_resolution_clock::now() - begin;
    std::cout << std::endl;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   Board:" << std::chrono::duration_cast<std::chrono::milliseconds>(last_dur).count() << "   <--->  ";
    std::cout << "BitBoard:" << std::chrono::duration_cast<std::chrono::milliseconds>(end).count() << std::endl;
    std::cout << "   Ratio:" << (float)last_dur.count()/end.count() << 'x' << std::endl;
    std::cout << "---------------------------------" << std::endl;
    return (float)last_dur.count()/end.count();
}

void BenchMark(){
   float value = 0;
   size_t tests = 20;
   for(size_t i = 0 ; i < tests ; i++)
       value+=BenchMarkOnce();

    std::cout << "---------------------------------" << std::endl;
    std::cout << "   Average Ratio:" << (float)value/tests << 'x' << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

void PrintBitBoard(uint64_t board){
    const char letters[] = " +";

    using namespace std;
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
    for(size_t i = 0 ; i<8;i++){
        cout << '|';
        for(size_t j = 0 ; j<8;j++){
            Position pos(i,j);
            cout << letters[((board >> pos.Value()) & 1) ] << "|";
        }
        cout << endl;
    }
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
}

uint64_t read_mask()
{
    uint64_t result = 0;
    for(size_t i = 0 ; i < 8 ; i++)
    {
        std::string line;
        std::getline(std::cin,line);
        for(size_t j = 0; j < 8 ; j++){
            if(line[j] == '1')
                result |= (1LL << (8*i+j));
        }
    }
    return result;
}

void PrintBoardIndexes(){
    const char letters[2][8] = {" pnbrqk"," PNBRQK"};

    using namespace std;

    for(size_t i = 0 ; i<8;i++){
        cout << "+--";
    }
    cout << "+" << endl;
    for(size_t i = 0 ; i<8;i++){
        cout << '|';
        for(size_t j = 0 ; j<8;j++){
            Position pos(i,j);
            if(pos.Value() < 10)
                cout << ' ';
            cout << (int)pos.Value() << "|";
        }
        cout << endl;
        for(size_t i = 0 ; i<8;i++){
            cout << "+--";
        }
        cout << "+" << endl;
    }

}

template <class T>
void PrintBoard(const T& board){

    const char letters[2][8] = {" pnbrqk"," PNBRQK"};

    using namespace std;
    for(size_t i = 0 ; i<17;i++){
        cout << "-";
    }
    cout << endl;
    for(size_t i = 0 ; i<8;i++){
        cout << '|';
        for(size_t j = 0 ; j<8;j++){
            Position pos(i,j);
            auto type = board.GetCell(pos);
            cout << letters[type.color][type.type] << "|";
        }
        cout << endl;
    }
    auto flags = board.RookingFlags();
    cout << (flags.white_oo ? 'K' : '-');
    cout << (flags.white_ooo ? 'Q' : '-');
    cout << (flags.black_oo ? 'k' : '-');
    cout << (flags.black_ooo ? 'q' : '-');
    for(size_t i = 0 ; i<13;i++){
        cout << "-";
    }

    cout << endl;

}

bool CompareUntillError(Board board , size_t depth){
    BitBoard bitboard(board.Fen());

    if(board.GenerateSubBoards().size() != bitboard.GenerateSubBoards().size()){
        PrintBoard(board);
        std::cout << board.Fen() << std::endl;
        std::cout << "Bitboard: "  << bitboard.GenerateSubBoards().size() << std::endl;
        std::cout << "Board: "  << board.GenerateSubBoards().size() << std::endl;

        for(auto board : bitboard.GenerateSubBoards()){
            PrintBoard(board);
        }

        return true;
    }
    if(depth==0)
        return false;

    for(auto sub : board.GenerateSubBoards()){
        if(CompareUntillError(sub,depth-1))
            return true;
    }

    return false;
}
bool CompareSubs(Board board,BitBoard bitboard , size_t depth){
    using namespace std;
    std::map<std::string,int> table;
    for(auto sub : board.GenerateSubBoards())
        table[sub.Fen()]++;
    for(auto sub : bitboard.GenerateSubBoards())
        table[sub.Fen()]++;
    for(auto pair : table)
        if(pair.second%2==0)
            table.erase(pair.first);
    if(table.size()==0)
        return false;
    else {
        for(auto pair : table)
            cout << pair.first << ":" << pair.second << endl;
        return true;
    }
}

bool CompareSubCount(Board board,BitBoard bitboard , size_t depth){
    using namespace std;
    std::map<std::pair<size_t,std::string>,int> table;
    for(auto sub : board.GenerateSubBoards()){
        cout << MovesCounter(sub,depth) << ":" << sub.Fen() << endl;
        table[{MovesCounter(sub,depth),sub.Fen()}]++;
    }
    cout << "---------------------" << endl;
    for(auto sub : bitboard.GenerateSubBoards()){
        table[{MovesCounter(sub,depth),sub.Fen()}]++;
        cout << MovesCounter(sub,depth) << ":" << sub.Fen() << endl;
    }
    cout << "---------------------" << endl;
    for(const auto &pair : table)
        if(pair.second%2==0)
            table.erase(pair.first);
    if(table.size()==0)
        return false;
    else {
        for(auto pair : table)
            cout <<pair.first.first << ":" << pair.first.second << ":" << pair.second << endl;
        return true;
    }
}

bool CompareSubC(Board board,BitBoard bitboard , size_t depth){
    using namespace std;
    std::map<std::string,size_t> table;
    std::map<std::string,size_t> bittable;
    for(auto sub : board.GenerateSubBoards()){
        table[sub.Fen()] = MovesCounter(sub,depth) ;
    }
    for(auto sub : bitboard.GenerateSubBoards()){
        bittable[sub.Fen()] = MovesCounter(sub,depth) ;
    }
    for(const auto &pair : bittable){
        cout << pair.first << endl;
        cout << pair.second << "<->";
        cout << table[pair.first] << "(:";

        auto turns = bitboard.GenerateSubBoards();

        for(size_t i = 0 ; i < turns.size(); i++)
            if(turns[i].Fen() == pair.first){
                cout << i ;
                break;
            }
        cout << ":)" << endl;
    }
    for(const auto &pair : table){
        cout << pair.first << endl;
        cout << pair.second << "<->";
        cout << bittable[pair.first] << endl;
    }
    return 0;
}

int main()
{
    //BenchMark();
   //SeeBoard(BitBoard(),6);
    //std::cout << MovesCounter(BitBoard(), 6);
    using namespace std;
    //PrintBitBoard(AttackFrom(1ULL << 35));
    /*
    Computer cmp(Color::kWhite);


    auto board = BitBoard();
    while(!board.End()){
        PrintBoard(board);
        board = cmp.GetTurn(board);

    }

*/
    BitBoard board;
    Computer cw(Color::kWhite);
    Computer cb(Color::kBlack);

    while(true)
    {
        board = cw.GetTurn(board);
        PrintBoard(board);
        std::cout << cw.Evaluate(board) << "<--->";
        std::cout << cw.EvaluateA(board) << std::endl;
        board = cb.GetTurn(board);
        PrintBoard(board);
        std::cout << cb.Evaluate(board) << "<--->";
        std::cout << cb.EvaluateA(board) << std::endl;
    }


    //PrintBoardIndexes();

    //BitBoard bitboard("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1" );
//    for(auto bit : bitboard.GenerateSubBoards())
 //       PrintBoard(bit);
    /*
    start :;

    size_t error_depth=1;


    PrintBoard(bitboard);
    //for(auto board : bitboard.GenerateSubBoards())
        //PrintBoard(board);
    CompareSubs(Board(bitboard.Fen()),bitboard,1);
    while(MovesCounter(Board(bitboard.Fen()),error_depth)==MovesCounter(bitboard,error_depth)){
        error_depth++;
        cout << "Board:----->" << MovesCounter(Board(bitboard.Fen()),error_depth) << endl;
        cout << "BitBoard:-->" << MovesCounter(bitboard,error_depth) << endl;
    }

    cout << "ERROR DEPTH: -->" << error_depth << endl;

    //CompareSubs(Board(string),BitBoard(string),3);
    CompareSubC(Board(bitboard.Fen()),bitboard,error_depth);

    //if(error_depth == 1)
        //PrintInside(bitboard);

    size_t num ;
    cin >> num;
    bitboard = bitboard.GenerateSubBoards()[num];


    goto start;
    */
/*

    cout << (MovesCounter(Board(string),2) == MovesCounter(BitBoard(string),2)) << endl;

    CompareSubCount(Board(string),BitBoard(string),1);

    */
    /*
    while(true){
        std::string bord;
        std::getline(std::cin,bord);
        cout << MovesCounter(Board(bord),1) << endl;
        cout << MovesCounter(BitBoard(bord),1) << endl;
    }
    */
    //BenchMark();
   // for(auto board : BitBoard("rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9").GenerateSubBoards()){
  //      PrintBoard(board);
  //  }
    //BitBoard board;
    //board = board.GenerateSubBoards()[0];
    //std::cout << (int)board.CurrentColor();

    //CompareUntillErrorSmart(Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"),3);
    /*
    */
    /*
    size_t depth;
    cin >> depth;
    SeeBoard(board,depth);
    while(true){
        int move;
        cin >> move;
        auto turn = board.GenerateTurns()[move];
        board.ExecuteTurn(turn);
        depth--;
    SeeBoard(board,depth);
    }
    */
    return 0;
}
