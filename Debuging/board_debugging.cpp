#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
#include <match.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <filesystem>
#include <openingsbase.hpp>
#include <position_rating.hpp>

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

std::string LoadFile(std::filesystem::path path){
    std::ifstream t(path);
    if(!t.is_open()){
        std::cerr << "Error while oppening file" << std::endl;
        exit(1);
    }

    size_t length;
    t.seekg(0, std::ios::end);    // go to the end
    length = t.tellg();           // report location (this is the length)
    t.seekg(0, std::ios::beg);    // go back to the beginning
    char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);       // read the whole file into the buffer

    std::string string(buffer,t.gcount());
    t.close();                    // close file handle
    delete[] buffer;
    return string;
}

int main()
{

    BitBoard board;

    auto begin = std::chrono::high_resolution_clock::now();
    for(int i = 0 ; i < 2000000 ; ++i)
        board = BitBoard(board.Fen());
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::duration<float>>(end-begin).count() << std::endl;


    //std::cout << read_mask();
    /*
    std::ios_base::sync_with_stdio(false);
    using namespace std;

    std::vector<std::filesystem::path> pathes;
    std::vector<Match> matches;

    for( auto && dir : filesystem::recursive_directory_iterator("matches"))
        if(dir.is_regular_file() && dir.path().filename().extension() == ".pgn")
            pathes.push_back(dir.path());


    for(size_t i = 0 ; i < pathes.size() ; i++)
    {
        //std::cout << pathes[i]<< " ";
        auto text = LoadFile(pathes[i]);
        auto begin = std::chrono::high_resolution_clock::now();
        auto match = Match::LoadFromPGN(text);
        std::copy(match.begin(), match.end(), std::back_inserter(matches));
        auto end = std::chrono::high_resolution_clock::now() - begin;
        cout << "Loaded: " << match.size() << " matches  ";
        cout << "Time: " << std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1,1>>>(end).count() << " s" << endl;
        cout << i+1 <<  '/' <<  pathes.size() << endl;
    }
    */
    //std::cout << MainAppraiser::Approximate(BitBoard("8/3P4/8/8/8/1p6/8/8 w - - 0 1"),Color::kWhite).ToString() << std::endl;

    /*
    OpeningsBase base;
    Match match;
    match.Push(Turn::FromChessFormat("a2a4"));
    auto nexts = base.FindNext(match);
    for(auto turn : nexts)
        std::cout << turn.ToChessFormat() << std::endl;
        */

    return 0;
}
