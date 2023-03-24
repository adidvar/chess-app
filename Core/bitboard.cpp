#include "bitboard.h"
#include <stdexcept>
#include <sstream>
#include <cassert>

const char* BitBoard::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BitBoard::BitBoard(std::string_view fen):
board_{{0},{0}}
{
    size_t i = 0;
    size_t position = 0;
    while(i < fen.size() && position != 64 && fen[i]!=' ')
    {
        switch (fen.at(i)) {
        case 'p':
            Set(position,{Figure::kPawn,Color::kBlack});
            position++;
            break;
        case 'n':
            Set(position,{Figure::kKnight,Color::kBlack});
            position++;
            break;
        case 'b':
            Set(position,{Figure::kBishop,Color::kBlack});
            position++;
            break;
        case 'r':
            Set(position,{Figure::kRook,Color::kBlack});
            position++;
            break;
        case 'q':
            Set(position,{Figure::kQueen,Color::kBlack});
            position++;
            break;
        case 'k':
            Set(position,{Figure::kKing,Color::kBlack});
            position++;
            break;
        case 'P':
            Set(position,{Figure::kPawn,Color::kWhite});
            position++;
            break;
        case 'N':
            Set(position,{Figure::kKnight,Color::kWhite});
            position++;
            break;
        case 'B':
            Set(position,{Figure::kBishop,Color::kWhite});
            position++;
            break;
        case 'R':
            Set(position,{Figure::kRook,Color::kWhite});
            position++;
            break;
        case 'Q':
            Set(position,{Figure::kQueen,Color::kWhite});
            position++;
            break;
        case 'K':
            Set(position,{Figure::kKing,Color::kWhite});
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
        last_pawn_move_ = Position('8'-pawn[1], pawn[0]-'a');

    if(current_move.front() == 'w')
        current_player_color_ = Color::kWhite;
    else if(current_move == "b")
        current_player_color_ = Color::kBlack;
    else
        throw std::runtime_error("fen invalid format [current_move]");
    passive_turn_counter_ = static_move;
    turn_counter_ = move_counter;

    if(current_player_color_ == Color::kWhite)
        last_pawn_move_ = Position(last_pawn_move_.x()+1,last_pawn_move_.y());
    else
        last_pawn_move_ = Position(last_pawn_move_.x()-1,last_pawn_move_.y());

    rooking_flags_ = {false,false,false,false};

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
    all_[0] = 0;
    all_[1] = 0;
    for(size_t i = 1 ; i < 7 ; i++){
        all_[0] |= board_[Color::kWhite][i];
        all_[1] |= board_[Color::kBlack][i];
    }
    board_[0][0] = ~(all_[0] | all_[1]);
    board_[1][0] = ~(all_[0] | all_[1]);
}

std::string BitBoard::Fen() const
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
        if(TestEmp(i))
            bypass_counter++;
        else
        {
            if(bypass_counter!=0)
            {
                buffer[position] = '0'+bypass_counter;
                position++;
                bypass_counter=0;
            }
            buffer[position] = symbols[GetColor(i)][GetFigure(i)];
            position++;
        }
    }

    if(bypass_counter!=0)
    {
        buffer[position] = '0'+bypass_counter;
        position++;
        bypass_counter=0;
    }

    std::string string(buffer);
    /*
    string += ' ';
    string.push_back( CurrentColor() == Color::kWhite ? 'w' : 'b');
    string += ' ';
    auto rooking_flags = RookingFlags();
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
    */
/*
    if(LastPawnMove().Valid())
        string += LastPawnMove().ToString();
    else
        string += '-';
    string += ' ';
    string += std::to_string(PassiveTurnCounter());
    string += ' ';
    string += std::to_string(TurnCounter());
    */


    return string;
}

Color BitBoard::CurrentColor() const noexcept
{
    return current_player_color_;
}

Color BitBoard::OpponentColor() const noexcept
{
    return current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
}

void BitBoard::Set(Position position, Cell cell) ///< Записує фігуру
{
    for(size_t i = 1 ; i < 7 ; i++)
    {
        board_[0][i] &= ~((uint64_t)1<<position.Value());
        board_[1][i] &= ~((uint64_t)1<<position.Value());
    }

    board_[cell.color][cell.type] |= ((uint64_t)1<<position.Value());

    all_[0] = 0;
    all_[1] = 0;
    for(size_t i = 1 ; i < 7 ; i++){
        all_[0] |= board_[Color::kWhite][i];
        all_[1] |= board_[Color::kBlack][i];
    }
    board_[1][0] = board_[0][0] = ~(all_[0] | all_[1]);
}

void BitBoard::Swap(Position p1, Position p2)
{
    assert(p1.Valid());
    assert(p2.Valid());
    auto cell = GetCell(p1);
    Set(p1,GetCell(p2));
    Set(p2,cell);
}

void BitBoard::UpdateState()
{

}

void BitBoard::SkipMove()
{
    current_player_color_ = OpponentColor();
}
bool BitBoard::Test(Figure figure, Position position) const noexcept
{
    assert(position.Valid());
    return ((board_[Color::kWhite][figure]  | (board_[Color::kBlack][figure])) >> position.Value()) & 1;
}

bool BitBoard::TestColor(Color color, Position position) const noexcept
{
    assert(position.Valid());
    return (all_[color] >> position.Value()) & 1;
}

bool BitBoard::TestEmp(Position position) const noexcept
{
    assert(position.Valid());
    return ( board_[0][0] >> position.Value()) & 1;
}
BitBoard::Cell BitBoard::GetCell(Position position) const noexcept
{
    assert(position.Valid());
    return {GetFigure(position),GetColor(position)};
}

Figure BitBoard::GetFigure(Position position) const noexcept
{
    assert(position.Valid());

    for(size_t i =0 ; i < 7 ; i++){
       if( Test(i,position) )
           return (i);
    }
}

Color BitBoard::GetColor(Position position) const noexcept
{
    assert(position.Valid());
    return TestColor(Color::kBlack,position);
}

constexpr static const uint64_t row_a = (1) + (1<<8) + (1<<16) + (1<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
constexpr static const uint64_t row_b = row_a << 1;
constexpr static const uint64_t row_c = row_a << 2;
constexpr static const uint64_t row_d = row_a << 3;
constexpr static const uint64_t row_e = row_a << 4;
constexpr static const uint64_t row_f = row_a << 5;
constexpr static const uint64_t row_g = row_a << 6;
constexpr static const uint64_t row_h = row_a << 7;

constexpr static const uint64_t line_8 = (1) + (1<<1) + (1<<2) + (1<<3) + (1LL<<4) + (1LL<<5) + (1LL<<6) + (1LL<<7);
constexpr static const uint64_t line_7 = line_8 << 8;
constexpr static const uint64_t line_6 = line_8 << 16;
constexpr static const uint64_t line_5 = line_8 << 24;
constexpr static const uint64_t line_4 = line_8 << 32;
constexpr static const uint64_t line_3 = line_8 << 40;
constexpr static const uint64_t line_2 = line_8 << 48;
constexpr static const uint64_t line_1 = line_8 << 56;

struct BitIterator{
    uint64_t value_;
    uint64_t bit_;

    constexpr BitIterator(uint64_t value):
        value_(value),bit_(0)
    {
        operator++();
    }

    constexpr uint64_t Value(){return value_;}
    constexpr uint64_t Bit(){return bit_;}
    constexpr void operator =(uint64_t value){value_ = value;operator++();}
    constexpr void operator++()
    {
        bit_ = value_^((value_-1)&value_);
        value_ &= ~bit_;
    }
    constexpr bool Valid(){
        return bit_!=0;
    }

};

template<bool color>
std::vector<BitBoard> BitBoard::GenerateSubBoardsTemplate() const
{
    std::vector<BitBoard> boards;

    Color my_color = color , op_color = !color;
    uint64_t empty = board_[my_color][Figure::kEmpty];
    uint64_t opponent = all_[op_color];
    uint64_t yours = all_[my_color];

    //pawns bro
    {
        uint64_t map = board_[my_color][Figure::kPawn];

        constexpr size_t move_direction = 8;

        uint64_t possible;
        if constexpr(color == Color::kWhite)
                possible = ((map >> move_direction) & empty) << move_direction;
        else
        possible = ((map << move_direction) & empty) >> move_direction;

        BitIterator iterator(possible);
        while(iterator.Valid())
        {
            uint64_t after;
            if constexpr(color == Color::kWhite)
                    after = iterator.Bit() >> move_direction;
            else
            after = iterator.Bit() << move_direction;

            uint64_t reverse = ~iterator.Bit();
            boards.emplace_back(*this);
            boards.back().board_[my_color][Figure::kPawn] &= reverse;
            boards.back().board_[my_color][Figure::kPawn] |= after;

            boards.back().all_[my_color] &= reverse;
            boards.back().all_[my_color] |= after;

            boards.back().board_[1][0] =
                    boards.back().board_[0][0] =
                    ~(boards.back().all_[0] | boards.back().all_[1]);

            ++iterator;
        }

        if constexpr(color == Color::kWhite)
                possible = (((map&(~row_a)) >> (move_direction+1)) & opponent) << (move_direction+1);
        else
        possible = (((map&(~row_h)) << (move_direction+1)) & opponent) >> (move_direction+1);
        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if constexpr(color == Color::kWhite)
                    after = iterator.Bit() >> (move_direction+1);
            else
            after = iterator.Bit() << (move_direction+1);

            uint64_t reverse = ~iterator.Bit();

            boards.emplace_back(*this);
            boards.back().board_[my_color][Figure::kPawn] &= reverse;
            boards.back().board_[my_color][Figure::kPawn] |= after;

            boards.back().all_[my_color] &= reverse;
            boards.back().all_[my_color] |= after;

            boards.back().board_[1][0] =
                    boards.back().board_[0][0] =
                    ~(boards.back().all_[0] | boards.back().all_[1]);

            for(size_t i = 1 ; i < 7;i++)
                boards.back().board_[op_color][i] &= reverse;
            boards.back().all_[op_color] &= reverse;

            ++iterator;
        }

        if constexpr(color == Color::kWhite)
            possible = (((map&(~row_h)) >> (move_direction-1)) & opponent) << (move_direction-1);
        else
            possible = (((map&(~row_a)) << (move_direction-1)) & opponent) >> (move_direction-1);

        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if constexpr(color == Color::kWhite)
                    after = iterator.Bit() >> (move_direction-1);
            else
            after = iterator.Bit() << (move_direction-1);

            uint64_t reverse = ~iterator.Bit();

            boards.emplace_back(*this);
            boards.back().board_[my_color][Figure::kPawn] &= reverse;
            boards.back().board_[my_color][Figure::kPawn] |= after;

            boards.back().all_[my_color] &= reverse;
            boards.back().all_[my_color] |= after;

            boards.back().board_[1][0] =
                    boards.back().board_[0][0] =
                    ~(boards.back().all_[0] | boards.back().all_[1]);

            for(size_t i = 1 ; i < 7;i++)
                boards.back().board_[op_color][i] &= reverse;
            boards.back().all_[op_color] &= reverse;

            ++iterator;
        }

        if constexpr(color == Color::kWhite)
                possible = (((((map & line_2) >> move_direction) & empty ) >> move_direction) & empty) << 2*move_direction;
        else
            possible = (((((map & line_7) << move_direction) & empty ) << move_direction) & empty) >> 2*move_direction;
        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if constexpr(color == Color::kWhite)
                    after = iterator.Bit() >> 2*move_direction;
            else
                after = iterator.Bit() << 2*move_direction;

            uint64_t reverse = ~iterator.Bit();

            boards.emplace_back(*this);
            boards.back().board_[my_color][Figure::kPawn] &= reverse;
            boards.back().board_[my_color][Figure::kPawn] |= after;

            boards.back().all_[my_color] &= reverse;
            boards.back().all_[my_color] |= after;

            boards.back().board_[1][0] =
                    boards.back().board_[0][0] =
                    ~(boards.back().all_[0] | boards.back().all_[1]);

            ++iterator;
        }
    }

    //knight bro
    {

    }


    return boards;
}

std::vector<BitBoard> BitBoard::GenerateSubBoards() const
{
    if(CurrentColor() == Color::kWhite)
        return GenerateSubBoardsTemplate<false>();
    else
        return GenerateSubBoardsTemplate<true>();
}


/*
BitBoard::operator bool() const noexcept
{
    return TurnGenerate(*this,current_color).size() != 0;
}
*/
