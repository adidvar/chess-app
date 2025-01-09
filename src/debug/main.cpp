#include <chesscore/bitboard.hpp>
#include <iostream>

#include <chrono>
#include <map>

#include "magic.hpp"

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

void print_bitboard(bitboard board)
{
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            bitboard mask = 1ULL << (rank * 8 + file);
            std::cout << ((board & mask) ? '1' : '0') << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    /*
    std::cout << "-----------------------" << std::endl;
    print_bitboard(1107298304);
    print_bitboard(processRook(27, 1107298304));
    print_bitboard(generateRookAttack(27, 1107298304));
*/

    // try {
    /*
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

*/
    return 0;
}
