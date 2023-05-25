#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <board.hpp>
#include <bitboard.hpp>
#include <magic.hpp>
//#include <computer.hpp>
#include <match.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <algorithm>

std::string LoadFile(std::string name){
    std::ifstream t(name);
    if(!t.is_open()){
        std::cerr << "Error while oppening file";
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
    std::ios_base::sync_with_stdio(false);
    using namespace std;

    auto text = LoadFile("Adams.pgn");
    auto matches = Match::LoadFromPGN(text);

    std::cout << matches.size() << std::endl;

    return 0;
}
