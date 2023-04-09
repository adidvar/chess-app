#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>

template<class Board>
static size_t MovesCounter(Board board,size_t depth){
    if(!depth)
        return 1;
    size_t counter = 0;

    for(auto board: board.GenerateSubBoards())
        counter += MovesCounter(board,depth-1);

    return counter;
}

void SeeBoard(Board board , size_t depth)
{
    using namespace std;
    for(auto turn : board.GenerateTurns()){
        Board copy(board);
        copy.ExecuteTurn(turn);
        cout << turn.ToChessFormat() << ": " << MovesCounter(copy,depth-1) << endl;
    }
    cout << MovesCounter(board,depth) << endl;
}


float BenchMarkOnce(){

    using namespace std::chrono;
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<Board>(Board(),3);
    auto end = std::chrono::high_resolution_clock::now() - begin;
    auto last_dur = end;

    begin = std::chrono::high_resolution_clock::now();
    std::cout << MovesCounter<BitBoard>(BitBoard(),3);
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
   size_t tests = 5;
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

void PrintInside(const BitBoard& board){

    for(size_t i = 0 ; i < 2; i++)
    for(size_t j = 0 ; j < 7; j++)
        PrintBitBoard(board.board_[i][j]);

    for(size_t i = 0 ; i < 2; i++)
        PrintBitBoard(board.all_[i]);
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
    /*
    for(const auto &pair : table){
        cout << pair.first << endl;
        cout << pair.second << "<->";
        cout << bittable[pair.first] << endl;
    }
    */
    return 0;
}

int main()
{
    BenchMark();
    using namespace std;
    /*
    InitMagic();
    PrintBoardIndexes();

    BitBoard bitboard("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");

    start :;

    size_t error_depth=1;


    PrintBoard(bitboard);
    for(auto board : bitboard.GenerateSubBoards())
        PrintBoard(board);
    CompareSubs(Board(bitboard.Fen()),bitboard,1);
    while(MovesCounter(Board(bitboard.Fen()),error_depth)==MovesCounter(bitboard,error_depth)){
        error_depth++;
        cout << "Board:----->" << MovesCounter(Board(bitboard.Fen()),error_depth) << endl;
        cout << "BitBoard:-->" << MovesCounter(bitboard,error_depth) << endl;
    }

    cout << "ERROR DEPTH: -->" << error_depth << endl;

    //CompareSubs(Board(string),BitBoard(string),3);
    CompareSubC(Board(bitboard.Fen()),bitboard,error_depth);

    if(error_depth == 1)
        PrintInside(bitboard);

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
