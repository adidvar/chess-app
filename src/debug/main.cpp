#include <chesscore/bitboard.hpp>
#include <iostream>

#include <chrono>
#include <map>

void PrintBoard(const BitBoard &board)
{
    const static std::map<Figure, char> f_to_s = {{Figure::Empty, ' '},
                                                  {Figure::BPawn, 'p'},
                                                  {Figure::BKnight, 'n'},
                                                  {Figure::BBishop, 'b'},
                                                  {Figure::BRook, 'r'},
                                                  {Figure::BQueen, 'q'},
                                                  {Figure::BKing, 'k'},
                                                  {Figure::WPawn, 'P'},
                                                  {Figure::WKnight, 'N'},
                                                  {Figure::WBishop, 'B'},
                                                  {Figure::WRook, 'R'},
                                                  {Figure::WQueen, 'Q'},
                                                  {Figure::WKing, 'K'}};

    using namespace std;
    for (size_t i = 0; i < 19; i++) {
        cout << "-";
    }
    cout << endl << "| |";
    for (size_t i = 0; i < 8; i++) {
        cout << char('A' + i) << '|';
    }
    cout << endl;
    for (size_t i = 0; i < 8; i++) {
        cout << '|' << char('8' - i) << '|';
        for (size_t j = 0; j < 8; j++) {
            Position pos(j, i);
            Figure type = board.get(pos);
            cout << f_to_s.at(type) << "|";
        }
        cout << endl;
    }
    auto flags = board.getFlags();
    cout << board.getTurn().toString() << '-';
    cout << (flags & BitBoard::flags_color ? 'B' : 'W') << '-';
    cout << (flags & BitBoard::flags_el_passant ? "el" : "--");
    cout << (flags & BitBoard::flags_white_oo ? 'K' : '-');
    cout << (flags & BitBoard::flags_white_ooo ? 'Q' : '-');
    cout << (flags & BitBoard::flags_black_oo ? 'k' : '-');
    cout << (flags & BitBoard::flags_black_ooo ? 'q' : '-');
    for (size_t i = 0; i < 6; i++) {
        cout << "-";
    }

    cout << endl;
    cout << endl;
}

static size_t Counter(const BitBoard &board, size_t depth)
{
    if (depth == 0)
        return 1;
    size_t counter = 0;
    Turn turns[216];
    int count = board.getTurns(board.getCurrentSide(), turns);
    if (depth == 1)
        return count;
    for (int i = 0; i < count; i++) {
        counter += Counter(board.executeTurn(board.getCurrentSide(), turns[i]), depth - 1);
    }
    return counter;
}

int main() {
    // try {
    while (true) {
        std::string fen;
        int depth = 7;
        std::cout << "***************************************************" << std::endl;
        //std::getline(std::cin, fen);
        //std::cin >> depth;
        auto board = BitBoard("startpos");
        //std::getline(std::cin, fen);
        PrintBoard(board);
        Turn turns[216];
        auto count = board.getTurns(board.getCurrentSide(), turns);
        long long sum = 0;

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < count; i++) {
            auto subboard = board.executeTurn(board.getCurrentSide(), turns[i]);
            auto subcount = Counter(subboard, depth - 1);
            std::cout << turns[i].toString() << '\t' << subcount << '\t' << subboard.fen()
                      << std::endl;
            sum += subcount;
        }

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "count: " << sum << std::endl;
        std::cout << "nps: "
                  << (long long int) (sum
                                      / (std::chrono::duration_cast<
                                             std::chrono::duration<float, std::ratio<1>>>(end
                                                                                          - start))
                                            .count())
                         / 1000000
                  << " millions" << std::endl;
    }
    //  } catch (std::exception e) {
    //      std::cout << e.what();
    //  }
}
