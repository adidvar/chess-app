#include "bitboard.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>
#include "magic.hpp"

const char* BitBoard::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BitBoard::BitBoard(std::string_view fen):
board_{{0},{0}}
{
    FenLoader<BitBoard> tools(*this);
    tools.LoadFromFen(fen);
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
    FenSaver<BitBoard> tools(*this);
    return tools.SaveToFen();
}

Color BitBoard::CurrentColor() const noexcept
{
    return current_player_color_;
}

Color BitBoard::OpponentColor() const noexcept
{
    return current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
}

BitBoard::RookingFlags_t BitBoard::RookingFlags() const noexcept
{
    return rooking_flags_;
}

size_t BitBoard::TurnCounter() const noexcept
{
    return turn_counter_;
}

size_t BitBoard::PassiveTurnCounter() const noexcept
{
    return passive_turn_counter_;
}

Position BitBoard::LastPawnMove() const noexcept
{
    return last_pawn_move_;
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

bool BitBoard::MateTest() const
{
    return (AttackMask(OpponentColor()) & board_[CurrentColor()][Figure::kKing]) != 0;
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

void BitBoard::Move(uint64_t from, uint64_t to, Color color, Figure type)
{
    board_[color][type] &= ~from;
    board_[color][type] |= to;

    all_[color] &= ~from;
    all_[color] |= to;

    board_[1][0] =
    board_[0][0] =
            ~(all_[0] | all_[1]);
}

void BitBoard::Attack(uint64_t from, uint64_t to, Color color, Figure type)
{
    board_[color][type] &= ~from;
    board_[color][type] |= to;

    all_[color] &= ~from;
    all_[color] |= to;

    for(size_t i = 1 ; i < 7;i++)
        board_[!color][i] &= ~to;
    all_[!color] &= ~to;

    board_[1][0] =
    board_[0][0] =
            ~(all_[0] | all_[1]);

}


struct Pawn{
};

struct Knight{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return
                (((figure << 10) & ~(row_a | row_b)) |
                 ((figure << 17) & (~row_a)) |
                 ((figure >> 6)) & ~(row_a | row_b) |
                 ((figure >> 15) & ~(row_a)) |
                 ((figure << 6)  & ~(row_g | row_h)) |
                 ((figure << 15) & ~(row_h)) |
                 ((figure >> 10) & ~(row_g | row_h)) |
                 ((figure >> 17) & ~(row_h) )) & ~allies
                ;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        return
                Generate(figure,all,allies);
                ;
    }
    constexpr static auto type = Figure::kKnight;
};
struct King{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return
                (((figure << 1) & ~row_a) |
                 ((figure << 9) & ~row_a) |
                 ((figure >> 7) & ~row_a) |
                 (figure >> 8) |
                 ((figure >> 1) & ~row_h) |
                 ((figure >> 9) & ~row_h) |
                 ((figure << 7) & ~row_h) |
                 (figure << 8)) & ~allies;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        return
                Generate(figure,all,allies);
                ;
    }
    constexpr static auto type = Figure::kKing;
};
struct Bishop{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return
                ProcessBishop(figure,all) & ~allies;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        uint64_t mask = 0;
        BitIterator iterator(figure);
        while(iterator.Valid()){
            mask |= Generate(iterator.Bit(),all,allies);
            ++iterator;
        }
        return mask;
    }
    constexpr static auto type = Figure::kBishop;
};
struct Rook{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return
                ProcessRook(figure,all) & ~allies;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        uint64_t mask = 0;
        BitIterator iterator(figure);
        while(iterator.Valid()){
            mask |= Generate(iterator.Bit(),all,allies);
            ++iterator;
        }
        return mask;
    }
    constexpr static auto type = Figure::kRook;
};
struct Queen{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return
                (ProcessBishop(figure,all) |
                 ProcessRook(figure,all)) & ~allies;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        uint64_t mask = 0;
        BitIterator iterator(figure);
        while(iterator.Valid()){
            mask |= Generate(iterator.Bit(),all,allies);
            ++iterator;
        }
        return mask;
    }
    constexpr static auto type = Figure::kQueen;
};


template<typename Type>
void BitBoard::ProcessFigure(std::vector<BitBoard> &boards, Color color, uint64_t from_mask, uint64_t to_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
{
    uint64_t map = board_[color][Type::type] & from_mask;
    BitIterator iterator(map);
    while(iterator.Valid())
    {
        uint64_t after = Type::Generate(iterator.Bit(),all,yours) & to_mask;
        after &= ~yours;
        uint64_t attack = after & opponent;
        after &= ~attack;

        BitIterator to(after);
        uint64_t reverse = ~iterator.Bit();
        while(to.Valid()){

            boards.emplace_back(*this);
            boards.back().Move(iterator.bit_,to.bit_,color,Type::type);

            ++to;
        }

        to = BitIterator(attack);
        while(to.Valid()){

            boards.emplace_back(*this);
            boards.back().Attack(iterator.bit_,to.bit_,color,Type::type);

            ++to;
        }
        ++iterator;
    }
}

template<>
void BitBoard::ProcessFigure<Pawn>(std::vector<BitBoard> &boards, Color color, uint64_t from_mask, uint64_t to_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
{
        uint64_t map = board_[color][Figure::kPawn] & from_mask;

        constexpr size_t move_direction = 8;

        uint64_t possible;
        if(color == Color::kWhite)
            possible = ((map >> move_direction) & ~all) << move_direction;
        else
            possible = ((map << move_direction) & ~all) >> move_direction;

        BitIterator iterator(possible);
        while(iterator.Valid())
        {
            uint64_t after;
            if(color == Color::kWhite)
                    after = iterator.Bit() >> move_direction;
            else
            after = iterator.Bit() << move_direction;

            boards.emplace_back(*this);
            boards.back().Move(iterator.Bit(),after,color,Figure::kPawn);

            ++iterator;
        }

        if (color == Color::kWhite)
                possible = (((map&(~row_a)) >> (move_direction+1)) & opponent) << (move_direction+1);
        else
                possible = (((map&(~row_h)) << (move_direction+1)) & opponent) >> (move_direction+1);
        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if(color == Color::kWhite)
                    after = iterator.Bit() >> (move_direction+1);
            else
            after = iterator.Bit() << (move_direction+1);

            boards.emplace_back(*this);
            boards.back().Attack(iterator.Bit(),after,color,Figure::kPawn);

            ++iterator;
        }

        if(color == Color::kWhite)
                possible = (((map&(~row_h)) >> (move_direction-1)) & opponent) << (move_direction-1);
        else
        possible = (((map&(~row_a)) << (move_direction-1)) & opponent) >> (move_direction-1);

        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if(color == Color::kWhite)
                    after = iterator.Bit() >> (move_direction-1);
            else
            after = iterator.Bit() << (move_direction-1);

            boards.emplace_back(*this);
            boards.back().Attack(iterator.Bit(),after,color,Figure::kPawn);

            ++iterator;
        }

        if(color == Color::kWhite)
            possible = (((((map & line_2) >> move_direction) & ~all ) >> move_direction) & ~all) << 2*move_direction;
        else
            possible = (((((map & line_7) << move_direction) & ~all ) << move_direction) & ~all) >> 2*move_direction;
        iterator = possible;

        while(iterator.Valid())
        {
            uint64_t after;
            if(color == Color::kWhite)
                    after = iterator.Bit() >> 2*move_direction;
            else
            after = iterator.Bit() << 2*move_direction;

            boards.emplace_back(*this);
            boards.back().Move(iterator.Bit(),after,color,Figure::kPawn);

            ++iterator;
        }
}

template<typename Type>
uint64_t BitBoard::ProcessAttack(Color color, uint64_t from_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
{
    uint64_t map = board_[color][Type::type] & from_mask;
    return Type::GenerateAttack(map,all,yours);
}

template<>
uint64_t BitBoard::ProcessAttack<Pawn>(Color color, uint64_t from_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
{
    uint64_t map = board_[color][Figure::kPawn] & from_mask;

    if (color == Color::kWhite)
    {
        return  (((map&(~row_a)) >> (8+1)) & opponent) |
                (((map&(~row_h)) >> (8-1)) & opponent) ;
    }
    else
    {
        return (((map&(~row_h)) << (8+1)) & opponent) |
               (((map&(~row_a)) << (8-1)) & opponent) ;
    }
}

uint64_t BitBoard::AttackMask(Color color) const
{
    uint64_t empty = board_[color][Figure::kEmpty];
    uint64_t opponent = all_[!color];
    uint64_t yours = all_[color];
    uint64_t all = ~empty;

    uint64_t result = 0;

    // pawns
    result |= ProcessAttack<Pawn>(color,~0ULL,all,yours,opponent);
    //knight
    result |= ProcessAttack<Knight>(color,~0ULL,all,yours,opponent);
    //king
    result |= ProcessAttack<King>(color,~0ULL,all,yours,opponent);
    // bishop
    result |= ProcessAttack<Bishop>(color,~0ULL,all,yours,opponent);
    // rook
    result |= ProcessAttack<Rook>(color,~0ULL,all,yours,opponent);
    // queen
    result |= ProcessAttack<Queen>(color,~0ULL,all,yours,opponent);

    return result;

}

std::vector<BitBoard> BitBoard::GenerateSubBoards(Color color) const
{
    std::vector<BitBoard> boards;

    uint64_t empty = board_[color][Figure::kEmpty];
    uint64_t opponent = all_[!color];
    uint64_t yours = all_[color];
    uint64_t all = ~empty;

    // pawns
    ProcessFigure<Pawn>(boards,color,~0ULL,~0ULL,all,yours,opponent);
    //knight
    ProcessFigure<Knight>(boards,color,~0ULL,~0ULL,all,yours,opponent);
    //king
    ProcessFigure<King>(boards,color,~0ULL,~AttackMask(!color),all,yours,opponent);
    // bishop
    ProcessFigure<Bishop>(boards,color,~0ULL,~0ULL,all,yours,opponent);
    // rook
    ProcessFigure<Rook>(boards,color,~0ULL,~0ULL,all,yours,opponent);
    // queen
    ProcessFigure<Queen>(boards,color,~0ULL,~0ULL,all,yours,opponent);

    auto it = std::remove_if(boards.begin(),boards.end(),[](const BitBoard&b){return b.MateTest();});
    boards.erase(it,boards.end());

    return boards;
}

std::vector<BitBoard> BitBoard::GenerateSubBoards() const
{
    if(CurrentColor() == Color::kWhite)
        return GenerateSubBoards(Color::kWhite);
    else
        return GenerateSubBoards(Color::kBlack);
}


/*
BitBoard::operator bool() const noexcept
{
    return TurnGenerate(*this,current_color).size() != 0;
}
*/
