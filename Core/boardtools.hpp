#ifndef BOARDTOOLS_H
#define BOARDTOOLS_H

#include <string_view>
#include <stdexcept>
#include <sstream>

#include "figures.hpp"
#include "position.hpp"

template<class Board>
class FenLoader{
    Board& board_;
public:
    FenLoader(Board &board):
        board_(board){}
    void LoadFromFen(std::string_view fen){
          for(size_t i = 0 ; i < 64 ; i++)
        board_.Set(i,{Figure::kEmpty,Color::kWhite});

    size_t i = 0;
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
    if(position!=64)
         throw std::runtime_error("fen invalid format [board]");

    std::stringstream ss;
    ss << fen.substr(i+1);

    std::string current_move , rooking , pawn;
    size_t static_move, move_counter;
    ss >> current_move >> rooking >> pawn >> static_move >> move_counter;

    if(pawn.size()==2)
        board_.last_pawn_move_ = Position(pawn[0]-'a','8'-pawn[1]);

    if(current_move.front() == 'w')
        board_.current_player_color_ = Color::kWhite;
    else if(current_move == "b")
        board_.current_player_color_ = Color::kBlack;
    else
        throw std::runtime_error("fen invalid format [current_move]");
    board_.passive_turn_counter_ = static_move;
    board_.turn_counter_ = move_counter;


    board_.rooking_flags_ = {false,false,false,false};

    for(char x : rooking)
    {
        switch (x) {
        case 'K':
            board_.rooking_flags_.white_oo = true;
            break;
        case 'Q':
            board_.rooking_flags_.white_ooo = true;
            break;
        case 'k':
            board_.rooking_flags_.black_oo = true;
            break;
        case 'q':
            board_.rooking_flags_.black_ooo = true;
            break;
        case '-':
            break;
        default:
            throw std::runtime_error("fen invalid format [rooking]");
        }
    }
    }
};

template<class Board>
class FenSaver{
    const Board& board_;
public:
    FenSaver(const Board &board):
        board_(board){}
    std::string SaveToFen() const {
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
    string += std::to_string(board_.PassiveTurnCounter());
    string += ' ';
    string += std::to_string(board_.TurnCounter());
    return string;
    }
};

#endif
