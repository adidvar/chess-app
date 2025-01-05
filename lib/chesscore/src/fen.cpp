#include "fen.hpp"
#include "bitboard.hpp"

#include <array>
#include <map>

constexpr static std::array<char, 3> seperators{' ', '\n', '\t'};

constexpr const std::string_view start_string = "startpos";

void skipSeperators(std ::string_view data, size_t &index)
{
    while (index < data.size()
           && std::find(seperators.begin(), seperators.end(), data[index]) != seperators.end())
        index++;
}

std::string_view readStringPart(std::string_view data, size_t &index)
{
    skipSeperators(data, index);
    auto begin = index;
    while (index < data.size()
           && std::find(seperators.begin(), seperators.end(), data[index]) == seperators.end())
        index++;
    auto end = index;
    skipSeperators(data, index);
    return data.substr(begin, end - begin);
}

void boardFromFen(std::string_view fen, BitBoard &board, size_t &index)
{
    const static std::map<char, Figure> s_to_f{{' ', Figure::Empty},
                                               {'p', Figure::BPawn},
                                               {'n', Figure::BKnight},
                                               {'b', Figure::BBishop},
                                               {'r', Figure::BRook},
                                               {'q', Figure::BQueen},
                                               {'k', Figure::BKing},
                                               {'P', Figure::WPawn},
                                               {'N', Figure::WKnight},
                                               {'B', Figure::WBishop},
                                               {'R', Figure::WRook},
                                               {'Q', Figure::WQueen},
                                               {'K', Figure::WKing}};

    skipSeperators(fen, index);

    if (fen.substr(index).starts_with(start_string)) {
        board = BitBoard{};
        index += start_string.size();
        return;
    }

    size_t position = 0;
    for (; index < fen.size() && position != 64; ++index) {
        const char character = fen.at(index);

        if (character == ' ')
            ;
        else if (s_to_f.contains(character)) {
            board = board.set(Position(position), s_to_f.at(character));
            position++;
        } else if (std::isdigit(character)) {
            if (int digit = character - '0'; digit > 0 && digit < 9)
                position += digit;
        } else if (character == '/')
            position = ((position - 1) / 8) * 8 + 8;
        else
            throw FenParsingError("invalid character");
    }
    ++index;

    if (position != 64)
        throw FenParsingError("incompleted fen");

    std::string current_move, rooking, pawn;
    std::string static_move, move_counter;

    current_move = readStringPart(fen, index);

    if (index == fen.size())
        throw FenParsingError("incompleted fen");

    rooking = readStringPart(fen, index);

    if (index == fen.size())
        throw FenParsingError("incompleted fen");

    pawn = readStringPart(fen, index);

    if (index == fen.size())
        throw FenParsingError("incompleted fen");

    static_move = readStringPart(fen, index);

    if (index == fen.size())
        throw FenParsingError("incompleted fen");

    move_counter = readStringPart(fen, index);

    BitBoard::Flags flags = BitBoard::flags_default;

    if (current_move.front() == 'w')
        ;
    else if (current_move == "b")
        flags = (BitBoard::Flags)(flags | BitBoard::flags_color);
    else
        throw FenParsingError("incorrect current side");

    if (pawn.size() == 2) {
        auto position = Position(pawn);
        if (!position.isValid())
            throw FenParsingError("incorrect el passant");
        if (flags & BitBoard::flags_color == 0) {
            board = board.setTurn(Turn(Position(pawn).index() - 8, Position(pawn).index() + 8));
        } else {
            board = board.setTurn(Turn(Position(pawn).index() + 8, Position(pawn).index() - 8));
        }
        flags = (BitBoard::Flags)(flags | BitBoard::flags_el_passant);
    }

    for (char x : rooking) {
        switch (x) {
        case 'K':
            flags = (BitBoard::Flags)(flags | BitBoard::flags_white_oo);
            break;
        case 'Q':
            flags = (BitBoard::Flags)(flags | BitBoard::flags_white_ooo);
            break;
        case 'k':
            flags = (BitBoard::Flags)(flags | BitBoard::flags_black_oo);
            break;
        case 'q':
            flags = (BitBoard::Flags)(flags | BitBoard::flags_black_ooo);
            break;
        case '-':
            break;
        default:
            throw FenParsingError("incorrect castling");
        }
    }
    board = board.setFlags(flags);
}

std::string boardToFen(const BitBoard &board)
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
    char buffer[128] = {0};
    size_t position = 0;
    size_t bypass_counter = 0;
    for (size_t i = 0; i < 64; i++) {
        if (i % 8 == 0 && i != 0) {
            if (bypass_counter != 0) {
                buffer[position] = '0' + bypass_counter;
                position++;
                bypass_counter = 0;
            }

            buffer[position] = '/';
            position++;
        }
        if (board.get(i).isEmpty())
            bypass_counter++;
        else {
            if (bypass_counter != 0) {
                buffer[position] = '0' + bypass_counter;
                position++;
                bypass_counter = 0;
            }
            buffer[position] = f_to_s.at(board.get(i));
            position++;
        }
    }

    if (bypass_counter != 0) {
        buffer[position] = '0' + bypass_counter;
        position++;
    }

    auto rooking_flags = board.getFlags();
    std::string string(buffer);
    string += ' ';
    string.push_back(rooking_flags & BitBoard::flags_color ? 'b' : 'w');
    string += ' ';
    if ((rooking_flags
         & (BitBoard::flags_black_oo | BitBoard::flags_black_ooo | BitBoard::flags_white_oo
            | BitBoard::flags_white_ooo))
        == 0)
        string.push_back('-');
    else {
        if (rooking_flags & BitBoard::flags_white_oo)
            string += 'K';
        if (rooking_flags & BitBoard::flags_white_ooo)
            string += 'Q';
        if (rooking_flags & BitBoard::flags_black_oo)
            string += 'k';
        if (rooking_flags & BitBoard::flags_black_ooo)
            string += 'q';
    }
    string += ' ';

    if (rooking_flags & BitBoard::flags_el_passant)
        string += Position((board.getTurn().from().index() + board.getTurn().to().index()) / 2)
                      .toString();
    else
        string += '-';

    string += ' ';
    string += std::to_string(0);
    string += ' ';
    string += std::to_string(0);
    return string;
}
