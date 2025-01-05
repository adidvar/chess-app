#include <chesscore/bitboard.hpp>
#include <chesscore/bitutils.hpp>
#include <iostream>

#include <map>

/*
template<typename labmda>
void BitCycle(BitBoard::bitboard bb, Lambda lambda)
{
    if (!bb)
        return;
    int count = popCount(bb);
    BitIterator it(bb);
    switch (count) {
    case 12:
        labmda(it.Bit());
        it++;
    case 11:
        labmda(it.Bit());
        it++;
    case 10:
        labmda(it.Bit());
        it++;
    case 9:
        labmda(it.Bit());
        it++;
    case 8:
        labmda(it.Bit());
        it++;
    case 7:
        labmda(it.Bit());
        it++;
    case 6:
        labmda(it.Bit());
        it++;
    case 5:
        labmda(it.Bit());
        it++;
    case 4:
        labmda(it.Bit());
        it++;
    case 3:
        labmda(it.Bit());
        it++;
    case 2:
        labmda(it.Bit());
        it++;
    case 1:
        labmda(it.Bit());
        it++;
    case 0:
        break;
    }
}
*/

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

int main() {
    try {
        std::string fen = "startpos";
        auto board = BitBoard(fen);
        PrintBoard(board);
        {
            auto subs = board.generateSubBoards(Color::White);
            std::cout << "count: " << subs.size() << std::endl;
            for (auto e : subs) {
                PrintBoard(e);
            }
        }
        /*
        {
            auto subs = BitBoard(fen).generateSubBoards(Color::Black);
            std::cout << "count: " << subs.size() << std::endl;
            for (auto e : subs) {
                PrintBoard(e);
            }
        }
*/
    } catch (std::exception e) {
        std::cout << e.what();
    }
}
