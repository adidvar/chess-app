#include "bitboard.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>
#include "magic.hpp"

constexpr uint64_t operator ""_b(uint64_t num){
    return (uint64_t)1 << num;
}

constexpr static const uint64_t row_a = 0_b + 8_b + 16_b + 24_b + 32_b + 40_b + 48_b + 56_b;
constexpr static const uint64_t row_b = row_a << 1;
constexpr static const uint64_t row_c = row_a << 2;
constexpr static const uint64_t row_d = row_a << 3;
constexpr static const uint64_t row_e = row_a << 4;
constexpr static const uint64_t row_f = row_a << 5;
constexpr static const uint64_t row_g = row_a << 6;
constexpr static const uint64_t row_h = row_a << 7;

constexpr static const uint64_t line_8 = 0_b + 1_b + 2_b + 3_b + 4_b + 5_b + 6_b + 7_b;
constexpr static const uint64_t line_7 = line_8 << 8;
constexpr static const uint64_t line_6 = line_8 << 16;
constexpr static const uint64_t line_5 = line_8 << 24;
constexpr static const uint64_t line_4 = line_8 << 32;
constexpr static const uint64_t line_3 = line_8 << 40;
constexpr static const uint64_t line_2 = line_8 << 48;
constexpr static const uint64_t line_1 = line_8 << 56;

const static uint64_t rooking_masks[2][2]{
    { 56_b+58_b+59_b+60_b , 60_b+61_b+62_b+63_b},
    { 0_b+2_b+3_b+4_b     , 4_b+5_b+6_b+7_b    }
};

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

const char* BitBoard::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

BitBoard::BitBoard(std::string_view fen):
board_{{0},{0}}
{
    FenLoader<BitBoard> tools(*this);
    tools.LoadFromFen(fen);

    all_[Color::kWhite] = all_[Color::kBlack] = 0;

    for(size_t i = 1 ; i < 7 ; i++)
    {
        all_[Color::kWhite] |= board_[Color::kWhite][i];
        all_[Color::kBlack] |= board_[Color::kBlack][i];
    }

    board_[Color::kWhite][Figure::kEmpty] = board_[Color::kBlack][Figure::kEmpty] = ~(all_[Color::kWhite] | all_[Color::kBlack]);
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
    return !current_player_color_;
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
        board_[Color::kWhite][i] &= ~((uint64_t)1<<position.Value());
        board_[Color::kBlack][i] &= ~((uint64_t)1<<position.Value());
    }

    board_[cell.color][cell.type] |= ((uint64_t)1<<position.Value());

    all_[Color::kWhite] = all_[Color::kBlack] = 0;

    for(size_t i = 1 ; i < 7 ; i++)
    {
        all_[Color::kWhite] |= board_[Color::kWhite][i];
        all_[Color::kBlack] |= board_[Color::kBlack][i];
    }

    board_[Color::kWhite][Figure::kEmpty] = board_[Color::kBlack][Figure::kEmpty] = ~(all_[Color::kWhite] | all_[Color::kBlack]);
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
    return ( board_[Color::kWhite][Figure::kEmpty] >> position.Value()) & 1;
}

BitBoard::Cell BitBoard::GetCell(Position position) const noexcept
{
    assert(position.Valid());
    return {GetFigure(position),GetColor(position)};
}

BitBoard::bitboard_t BitBoard::GetBitBoard(Color color, Figure figure) const noexcept
{
    return board_[color][figure];
}

bool BitBoard::MateTest() const
{
    return (AttackMask(OpponentColor()) & board_[CurrentColor()][Figure::kKing]) != 0;
}

bool BitBoard::End() const
{
    return GenerateSubBoards().size() == 0;
}

bool BitBoard::Checkmate() const
{
    return End() && MateTest();
}

bool BitBoard::WhiteWin() const
{
    return (CurrentColor() == Color::kBlack) && End();
}

bool BitBoard::BlackWin() const
{
    return (CurrentColor() == Color::kWhite) && End();
}

bool BitBoard::Tie() const
{
    return End() && !MateTest();
}

bool BitBoard::OpponentMateTest() const
{
    return (AttackMask(CurrentColor()) & board_[OpponentColor()][Figure::kKing]) != 0;
}

Figure BitBoard::GetFigure(Position position) const noexcept
{
    assert(position.Valid());

    for(size_t i =0 ; i < 7 ; i++){
       if( Test(i,position) )
           return (i);
    }

    return Figure::kEmpty;
}

Color BitBoard::GetColor(Position position) const noexcept
{
    assert(position.Valid());
    return TestColor(Color::kBlack,position);
}


void BitBoard::Move(uint64_t from, uint64_t to, Color color, Figure type)
{
    board_[color][type] &= ~from;
    board_[color][type] |= to;

    all_[color] &= ~from;
    all_[color] |= to;

    board_[Color::kWhite][Figure::kEmpty] = board_[Color::kBlack][Figure::kEmpty] = ~(all_[Color::kWhite] | all_[Color::kBlack]);

    if(from & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b))
    {
        if(from & 0_b )
            rooking_flags_.black_ooo = false;
        else if(from & 4_b ){
            rooking_flags_.black_ooo = false;
            rooking_flags_.black_oo = false;
        }
        else if(from & 7_b )
            rooking_flags_.black_oo = false;
        else if(from & 56_b )
            rooking_flags_.white_ooo = false;
        else if(from & 60_b ){
            rooking_flags_.white_ooo = false;
            rooking_flags_.white_oo = false;
        }
        else if(from & 63_b )
            rooking_flags_.white_oo = false;
    }
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

    board_[Color::kWhite][Figure::kEmpty] = board_[Color::kBlack][Figure::kEmpty] = ~(all_[Color::kWhite] | all_[Color::kBlack]);

    if(to & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b))
    {
        if(to & 0_b )
            rooking_flags_.black_ooo = false;
        else if(to & 4_b ){
            rooking_flags_.black_ooo = false;
            rooking_flags_.black_oo = false;
        }
        else if(to & 7_b )
            rooking_flags_.black_oo = false;
        else if(to & 56_b )
            rooking_flags_.white_ooo = false;
        else if(to & 60_b ){
            rooking_flags_.white_ooo = false;
            rooking_flags_.white_oo = false;
        }
        else if(to & 63_b )
            rooking_flags_.white_oo = false;
    }
    if(from & (0_b | 4_b | 7_b | 56_b | 60_b | 63_b))
    {
        if(from & 0_b )
            rooking_flags_.black_ooo = false;
        else if(from & 4_b ){
            rooking_flags_.black_ooo = false;
            rooking_flags_.black_oo = false;
        }
        else if(from & 7_b )
            rooking_flags_.black_oo = false;
        else if(from & 56_b )
            rooking_flags_.white_ooo = false;
        else if(from & 60_b ){
            rooking_flags_.white_ooo = false;
            rooking_flags_.white_oo = false;
        }
        else if(from & 63_b )
            rooking_flags_.white_oo = false;
    }

}

void BitBoard::Transform(uint64_t sq, Color color, Figure from, Figure to)
{
    board_[color][from] &= ~sq;
    board_[color][to] |= sq;
}


struct Pawn{
};

struct Knight{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return GenerateAttack(figure,all,allies) & ~allies
                ;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        return
                (((figure << 10) & ~(row_a | row_b)) |
                 ((figure << 17) & (~row_a)) |
                 ((figure >> 6)) & ~(row_a | row_b) |
                 ((figure >> 15) & ~(row_a)) |
                 ((figure << 6)  & ~(row_g | row_h)) |
                 ((figure << 15) & ~(row_h)) |
                 ((figure >> 10) & ~(row_g | row_h)) |
                 ((figure >> 17) & ~(row_h) )) ;
                ;
    }
    constexpr static auto type = Figure::kKnight;
};
struct King{
    static uint64_t Generate(uint64_t figure, uint64_t all, uint64_t allies){
        return GenerateAttack(figure,all,allies) & ~allies
                ;
    }
    static uint64_t GenerateAttack(uint64_t figure, uint64_t all, uint64_t allies){
        return
                (((figure << 1) & ~row_a) |
                 ((figure << 9) & ~row_a) |
                 ((figure >> 7) & ~row_a) |
                 (figure >> 8) |
                 ((figure >> 1) & ~row_h) |
                 ((figure >> 9) & ~row_h) |
                 ((figure << 7) & ~row_h) |
                 (figure << 8));
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
            mask |= ProcessBishop(iterator.Bit(),all);
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
            mask |= ProcessRook(iterator.Bit(),all);
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
            mask |= (ProcessBishop(iterator.Bit(),all) |
                 ProcessRook(iterator.Bit(),all));
            ++iterator;
        }
        return mask;
    }
    constexpr static auto type = Figure::kQueen;
};


template<typename Type>
void BitBoard::ProcessFigure(const BitBoard &parrent, std::vector<BitBoard> &boards, Color color, uint64_t from_mask, uint64_t to_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
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

            boards.emplace_back(parrent);
            boards.back().Move(iterator.bit_,to.bit_,color,Type::type);

            ++to;
        }

        to = BitIterator(attack);
        while(to.Valid()){

            boards.emplace_back(parrent);
            boards.back().Attack(iterator.bit_,to.bit_,color,Type::type);

            ++to;
        }
        ++iterator;
    }
}

template<>
void BitBoard::ProcessFigure<Pawn>(const BitBoard &parrent, std::vector<BitBoard> &boards, Color color, uint64_t from_mask, uint64_t to_mask, uint64_t all, uint64_t yours, uint64_t opponent) const
{
    uint64_t map = board_[color][Figure::kPawn] & from_mask;

    uint64_t possible;
    if(color == Color::kWhite)
        possible = (((map >> 8) & ~all) & to_mask) << 8;
    else
        possible = (((map << 8) & ~all) & to_mask) >> 8;

    BitIterator iterator(possible);
    while(iterator.Valid())
    {
        uint64_t after;
        if(color == Color::kWhite)
            after = iterator.Bit() >> 8;
        else
            after = iterator.Bit() << 8;

        BitBoard copy(parrent);
        copy.Move(iterator.Bit(),after,color,Figure::kPawn);
        boards.emplace_back(copy);
        if(after & (line_1|line_8))
        {
            boards.back().Transform(after,color,Figure::kPawn,Figure::kQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kRook);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kKnight);
        }

        ++iterator;
    }

    if (color == Color::kWhite)
        possible = (((map&(~row_a)) >> (8+1)) & opponent & to_mask) << (8+1);
    else
        possible = (((map&(~row_h)) << (8+1)) & opponent & to_mask) >> (8+1);
    iterator = possible;

    while(iterator.Valid())
    {
        uint64_t after;
        if(color == Color::kWhite)
            after = iterator.Bit() >> (8+1);
        else
            after = iterator.Bit() << (8+1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(),after,color,Figure::kPawn);
        boards.emplace_back(copy);
        if(after & (line_1|line_8))
        {
            boards.back().Transform(after,color,Figure::kPawn,Figure::kQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kRook);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kKnight);
        }

        ++iterator;
    }

    if(color == Color::kWhite)
        possible = (((map&(~row_h)) >> (8-1)) & opponent & to_mask) << (8-1);
    else
        possible = (((map&(~row_a)) << (8-1)) & opponent & to_mask) >> (8-1);

    iterator = possible;

    while(iterator.Valid())
    {
        uint64_t after;
        if(color == Color::kWhite)
            after = iterator.Bit() >> (8-1);
        else
            after = iterator.Bit() << (8-1);

        BitBoard copy(parrent);
        copy.Attack(iterator.Bit(),after,color,Figure::kPawn);
        boards.emplace_back(copy);
        if(after & (line_1|line_8))
        {
            boards.back().Transform(after,color,Figure::kPawn,Figure::kQueen);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kRook);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kBishop);
            boards.emplace_back(copy);
            boards.back().Transform(after,color,Figure::kPawn,Figure::kKnight);
        }

        ++iterator;
    }

    if(color == Color::kWhite)
        possible = (((((map & line_2) >> 8) & ~all ) >> 8) & ~all & to_mask) << 2*8;
    else
        possible = (((((map & line_7) << 8) & ~all ) << 8) & ~all & to_mask) >> 2*8;
    iterator = possible;

    while(iterator.Valid())
    {
        uint64_t after;
        uint64_t el_passant;
        if(color == Color::kWhite){
            after = iterator.Bit() >> 2*8;
            el_passant = iterator.Bit() >> 8;
        }
        else{
            after = iterator.Bit() << 2*8;
            el_passant = iterator.Bit() << 8;
        }

        boards.emplace_back(parrent);
        boards.back().last_pawn_move_ = log2_64(el_passant);
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
        return  (((map&(~row_a)) >> (8+1))) |
                (((map&(~row_h)) >> (8-1))) ;
    }
    else
    {
        return (((map&(~row_h)) << (8+1))) |
               (((map&(~row_a)) << (8-1))) ;
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

void BitBoard::GenerateSubBoards(Color color, std::vector<BitBoard>& boards) const
{
    boards.clear();

    uint64_t empty = board_[color][Figure::kEmpty];
    uint64_t opponent = all_[!color];
    uint64_t yours = all_[color];
    uint64_t all = ~empty;
    uint64_t attack = AttackMask(!color);

    BitBoard parent(*this);
    parent.last_pawn_move_ = Position();
    parent.SkipMove();



    uint64_t locked_figures = attack & yours & AttackFrom(board_[color][Figure::kKing]);

    // pawns
    ProcessFigure<Pawn>(parent,boards,color,~locked_figures,~0ULL,all,yours,opponent);
    //knight
    ProcessFigure<Knight>(parent,boards,color,~locked_figures,~0ULL,all,yours,opponent);
    // bishop
    ProcessFigure<Bishop>(parent,boards,color,~locked_figures,~0ULL,all,yours,opponent);
    // rook
    ProcessFigure<Rook>(parent,boards,color,~locked_figures,~0ULL,all,yours,opponent);
    // queen
    ProcessFigure<Queen>(parent,boards,color,~locked_figures,~0ULL,all,yours,opponent);
    //king
    ProcessFigure<King>(parent,boards,color,~locked_figures,~attack,all,yours,opponent);

    auto index = boards.size();
    if((attack & board_[color][Figure::kKing]) != 0)
        index = 0;

    // pawns
    ProcessFigure<Pawn>(parent,boards,color,locked_figures,~0ULL,all,yours,opponent);
    //knight
    ProcessFigure<Knight>(parent,boards,color,locked_figures,~0ULL,all,yours,opponent);
    // bishop
    ProcessFigure<Bishop>(parent,boards,color,locked_figures,~0ULL,all,yours,opponent);
    // rook
    ProcessFigure<Rook>(parent,boards,color,locked_figures,~0ULL,all,yours,opponent);
    // queen
    ProcessFigure<Queen>(parent,boards,color,locked_figures,~0ULL,all,yours,opponent);
    //king
    ProcessFigure<King>(parent,boards,color,locked_figures,~attack,all,yours,opponent);

    if(last_pawn_move_.Valid() && CurrentColor() == color)
    {
        uint64_t sq = 1ULL << last_pawn_move_.Value();
        uint64_t pawns = board_[color][Figure::kPawn];
        if (color == Color::kWhite)
        {
            uint64_t possible = ((((pawns&(~row_a)) >> (8+1)) & sq) ) << (8+1);
            if(possible){
                boards.emplace_back(parent);
                boards.back().Attack(possible,sq << 8,color,Figure::kPawn);
                boards.back().Move(sq << 8,sq,color,Figure::kPawn);
            }
            possible = ((((pawns&(~row_h)) >> (8-1)) & sq) ) << (8-1);
            if(possible){
                boards.emplace_back(parent);
                boards.back().Attack(possible,sq << 8,color,Figure::kPawn);
                boards.back().Move(sq << 8,sq,color,Figure::kPawn);
            }
        }
        else
        {
            uint64_t possible = ((((pawns&(~row_h)) << (8+1)) & sq)) >> (8+1);
            if(possible){
                boards.emplace_back(parent);
                boards.back().Attack(possible,sq >> 8,color,Figure::kPawn);
                boards.back().Move(sq >> 8,sq,color,Figure::kPawn);
            }
            possible = ((((pawns&(~row_a)) << (8-1)) & sq) ) >> (8-1);
            if(possible){
                boards.emplace_back(parent);
                boards.back().Attack(possible,sq >> 8,color,Figure::kPawn);
                boards.back().Move(sq >> 8,sq,color,Figure::kPawn);
            }
        }
    }

    auto it = std::remove_if(boards.begin()+index,boards.end(),[](const BitBoard&b){return b.OpponentMateTest();});
    boards.erase(it,boards.end());

    //rooking
    if (color == Color::kWhite){

        uint64_t right = rooking_flags_.white_oo;
        right = (right << 63) & board_[Color::kWhite][Figure::kRook];
        right >>= 1;
        right &= (empty & ~attack);
        right >>= 1;
        right &= (empty & ~attack);
        right >>= 1;
        right &= (~attack) & board_[Color::kWhite][Figure::kKing];

        uint64_t left = rooking_flags_.white_ooo;
        left = (left << 56) & board_[Color::kWhite][Figure::kRook];
        left <<= 1;
        left &= empty;
        left <<= 1;
        left &= (empty & ~attack);
        left <<= 1;
        left &= (empty & ~attack);
        left <<= 1;
        left &= (~attack) & board_[Color::kWhite][Figure::kKing];

        if(right != 0)
        {
            boards.emplace_back(parent);
            boards.back().Move(1ULL << 60, 1ULL << 62,Color::kWhite,Figure::kKing);
            boards.back().Move(1ULL << 63, 1ULL << 61,Color::kWhite,Figure::kRook);
            boards.back().rooking_flags_.white_oo = false;
            boards.back().rooking_flags_.white_ooo = false;
        }
        if(left != 0)
        {
            boards.emplace_back(parent);
            boards.back().Move(1ULL << 60, 1ULL << 58,Color::kWhite,Figure::kKing);
            boards.back().Move(1ULL << 56, 1ULL << 59,Color::kWhite,Figure::kRook);
            boards.back().rooking_flags_.white_oo = false;
            boards.back().rooking_flags_.white_ooo = false;
        }
    } else {
        uint64_t right = rooking_flags_.black_oo;
        right = (right << 7) & board_[Color::kBlack][Figure::kRook];
        right >>= 1;
        right &= (empty & ~attack);
        right >>= 1;
        right &= (empty & ~attack);
        right >>= 1;
        right &= (~attack) & board_[Color::kBlack][Figure::kKing];

        uint64_t left = rooking_flags_.black_ooo;
        left = (left << 0) & board_[Color::kBlack][Figure::kRook];
        left <<= 1;
        left &= empty;
        left <<= 1;
        left &= (empty & ~attack);
        left <<= 1;
        left &= (empty & ~attack);
        left <<= 1;
        left &= (~attack) & board_[Color::kBlack][Figure::kKing];

        if(right != 0)
        {
            boards.emplace_back(parent);
            boards.back().Move(1ULL << 4, 1ULL << 6,Color::kBlack,Figure::kKing);
            boards.back().Move(1ULL << 7, 1ULL << 5,Color::kBlack,Figure::kRook);
            boards.back().rooking_flags_.black_oo = false;
            boards.back().rooking_flags_.black_ooo = false;
        }
        if(left != 0)
        {
            boards.emplace_back(parent);
            boards.back().Move(1ULL << 4, 1ULL << 2,Color::kBlack,Figure::kKing);
            boards.back().Move(1ULL << 0, 1ULL << 3,Color::kBlack,Figure::kRook);
            boards.back().rooking_flags_.black_oo = false;
            boards.back().rooking_flags_.black_ooo = false;
        }

    }
}

std::vector<BitBoard> BitBoard::GenerateSubBoards() const
{
    std::vector<BitBoard> boards;
    boards.reserve(120);

    GenerateSubBoards(CurrentColor(),boards);
    return boards;
}

std::vector<BitBoard> BitBoard::GenerateSubBoards(Color color) const
{
    std::vector<BitBoard> boards;
    boards.reserve(120);

    GenerateSubBoards(color,boards);
    return boards;
}

void BitBoard::GenerateSubBoards(std::vector<BitBoard> &boards) const
{
    GenerateSubBoards(CurrentColor(),boards);
}

void BitBoard::GenerateSubBoards(std::vector<BitBoard> &boards, Color color) const
{
    GenerateSubBoards(color,boards);
}

std::vector<Turn> BitBoard::GenerateTurns(Color color) const
{
    std::vector<BitBoard> boards;
    std::vector<Turn> turns;
    boards.reserve(120);
    GenerateSubBoards(boards,color);

    for(auto subboard : boards){
        uint64_t delta = all_[color] ^ subboard.all_[color];
        if(rooking_masks[color][0] == delta){
            turns.push_back(color == Color::kWhite ? Turn(60,58) : Turn(4,2));
        }
        else if(rooking_masks[color][1] == delta){
            turns.push_back(color == Color::kWhite ? Turn(60,62) : Turn(4,6));
        }
        else if(count_1s(board_[color][Figure::kPawn]) != count_1s(subboard.board_[color][Figure::kPawn])){
            uint64_t from = all_[color] & delta;
            uint64_t to = subboard.all_[color] & delta;
            turns.push_back(Turn(log2_64(from),log2_64(to),subboard.GetFigure(log2_64(to))));
        }
        else {
            uint64_t from = all_[color] & delta;
            uint64_t to = subboard.all_[color] & delta;
            turns.push_back(Turn(log2_64(from),log2_64(to)));
        }
    }
    return turns;
}


std::vector<Turn> BitBoard::GenerateTurns() const
{
    return GenerateTurns(CurrentColor());
}

bool BitBoard::ExecuteTurn(Turn turn)
{
    auto turns = GenerateTurns();
    auto maps = GenerateSubBoards();
    for(size_t i = 0 ; i < turns.size() ; ++i){
        if(turns[i] == turn)
        {
            *this = maps[i];
            return true;
        }
    }
    return false;
}

bool BitBoard::TestTurn(Turn turn) const
{
    auto turns = GenerateTurns();
    return std::count(turns.begin(),turns.end(),turn) == 1;
}
