#include "parsingtools.hpp"

#include <stdexcept>
#include <sstream>

#include <bitboard.hpp>

void LoadFromFen(std::string_view fen, BitBoard &board_, size_t &index)
{
    for(size_t i = 0 ; i < Position::Max() ; i++)
        board_.Set(i,{Figure::kEmpty,Color::kWhite});

    size_t i = index;
    size_t position = 0;
    while(i < fen.size() && position != 64 && fen[i]!=' ')
    {
        switch (fen.at(i)) {
        case 'p':
            board_.Set(position,{Figure::kPawn,Color::kBlack});
            position++;
            break;
        case 'n':
            board_.Set(position,{Figure::kKnight,Color::kBlack});
            position++;
            break;
        case 'b':
            board_.Set(position,{Figure::kBishop,Color::kBlack});
            position++;
            break;
        case 'r':
            board_.Set(position,{Figure::kRook,Color::kBlack});
            position++;
            break;
        case 'q':
            board_.Set(position,{Figure::kQueen,Color::kBlack});
            position++;
            break;
        case 'k':
            board_.Set(position,{Figure::kKing,Color::kBlack});
            position++;
            break;
        case 'P':
            board_.Set(position,{Figure::kPawn,Color::kWhite});
            position++;
            break;
        case 'N':
            board_.Set(position,{Figure::kKnight,Color::kWhite});
            position++;
            break;
        case 'B':
            board_.Set(position,{Figure::kBishop,Color::kWhite});
            position++;
            break;
        case 'R':
            board_.Set(position,{Figure::kRook,Color::kWhite});
            position++;
            break;
        case 'Q':
            board_.Set(position,{Figure::kQueen,Color::kWhite});
            position++;
            break;
        case 'K':
            board_.Set(position,{Figure::kKing,Color::kWhite});
            position++;
            break;
        case '1':
            position+=1;
            break;
        case '2':
            position+=2;
            break;
        case '3':
            position+=3;
            break;
        case '4':
            position+=4;
            break;
        case '5':
            position+=5;
            break;
        case '6':
            position+=6;
            break;
        case '7':
            position+=7;
            break;
        case '8':
            position+=8;
            break;
        case '/':
            position = ((position-1)/8)*8+8  ;
            break;
        case ' ':
            break;
        default:
            throw std::runtime_error("fen invalid format [board]");
            break;
        }
        ++i;
    }
    ++i;

    if(position!=64)
        throw std::runtime_error("fen invalid format [board]");

    std::string current_move , rooking , pawn;
    std::string static_move, move_counter;

    current_move = ReadUntillDelims(fen,{' '},i);
    rooking = ReadUntillDelims(fen,{' '},i);
    pawn = ReadUntillDelims(fen,{' '},i);

    static_move =  ReadUntillDelims(fen,{' '},i);
    move_counter =  ReadUntillDelims(fen,{' '},i);

    if(pawn.size()==2)
        board_.SetLastPawnMove(Position::FromString(pawn));

    if(current_move.front() == 'w')
        board_.SetCurrentColor(Color::kWhite);
    else if(current_move == "b")
        board_.SetCurrentColor(Color::kBlack);
    else
        throw std::runtime_error("fen invalid format [current_move]");

    struct BitBoard::RookingFlagsT rooking_flags_ = {false,false,false,false};

    for(char x : rooking)
    {
        switch (x) {
        case 'K':
            rooking_flags_.white_oo = true;
            break;
        case 'Q':
            rooking_flags_.white_ooo = true;
            break;
        case 'k':
            rooking_flags_.black_oo = true;
            break;
        case 'q':
            rooking_flags_.black_ooo = true;
            break;
        case '-':
            break;
        default:
            throw std::runtime_error("fen invalid format [rooking]");
        }
    }
    board_.SetRookingFlags(rooking_flags_);
}

std::string SaveToFen(BitBoard board_)
{
    const char *symbols[2] = {
        " PNBRQK",
        " pnbrqk"
    };
    char buffer[128] = {0};
    size_t position = 0;
    size_t bypass_counter = 0;
    for(size_t i = 0 ; i < 64; i++){
        if(i%8==0 && i!=0)
        {
            if(bypass_counter!=0)
            {
                buffer[position] = '0'+bypass_counter;
                position++;
                bypass_counter=0;
            }

            buffer[position] = '/';
            position++;

        }
        if(board_.TestEmp(i))
            bypass_counter++;
        else
        {
            if(bypass_counter!=0)
            {
                buffer[position] = '0'+bypass_counter;
                position++;
                bypass_counter=0;
            }
            buffer[position] = symbols[board_.GetColor(i)][board_.GetFigure(i)];
            position++;
        }
    }

    if(bypass_counter!=0)
    {
        buffer[position] = '0'+bypass_counter;
        position++;
    }

    std::string string(buffer);
    string += ' ';
    string.push_back( board_.CurrentColor() == Color::kWhite ? 'w' : 'b');
    string += ' ';
    auto rooking_flags = board_.RookingFlags();
    if(rooking_flags.white_oo == false &&
            rooking_flags.white_ooo == false &&
            rooking_flags.black_oo == false &&
            rooking_flags.black_ooo == false)
        string.push_back('-');
    else{
        if(rooking_flags.white_oo)
            string +='K';
        if(rooking_flags.white_ooo)
            string +='Q';
        if(rooking_flags.black_oo)
            string +='k';
        if(rooking_flags.black_ooo)
            string +='q';
    }
    string += ' ';

    if(board_.LastPawnMove().Valid())
        string += board_.LastPawnMove().ToString();
    else
        string += '-';
    string += ' ';
    string += std::to_string(0);
    string += ' ';
    string += std::to_string(0);
    return string;
}

std::string_view ReadUntillDelims(std::string_view data, const std::vector<char> &seperators, size_t &index)
{
    auto prefix_size = index;
    data.remove_prefix(prefix_size);

    index = 0;

    while(std::find(seperators.begin(),seperators.end(),data[index]) != seperators.end())
        index++;

    auto begin = index;

    while(index < data.size() && std::find(seperators.begin(),seperators.end(),data[index]) == seperators.end() )
        index++;

    auto end = index;

    while(index < data.size() && std::find(seperators.begin(),seperators.end(),data[index]) != seperators.end() )
        index++;

    index += prefix_size;

    return data.substr( begin, end-begin );
}

std::vector<std::string_view> SplitByDelims(std::string_view data, const std::vector<char> &seperators){

    std::vector<std::string_view> turns_pre;
    turns_pre.reserve(100);

    size_t index = 0;
    size_t last = 0;
    while (index != data.size()) {
        for (auto sep : seperators)
            if (data[index] == sep) {
                if (index != last) {
                   turns_pre.push_back(data.substr(last, index - last));
                }
                last = index+1;
                break;
            }
        index++;
    }
    turns_pre.push_back(data.substr(last));

    return turns_pre;
}
