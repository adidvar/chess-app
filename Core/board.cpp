#include "board.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>


const char* Board::kStartPosition_ = u8"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

Board::Board(std::string_view fen)
{
    FenLoader<Board> tools(*this);
    tools.LoadFromFen(fen);

    if(current_player_color_ == Color::kWhite)
        last_pawn_move_ = Position(last_pawn_move_.x()+1,last_pawn_move_.y());
    else
        last_pawn_move_ = Position(last_pawn_move_.x()-1,last_pawn_move_.y());
}

std::string Board::Fen() const
{
    FenSaver<Board> tools(*this);
    return tools.SaveToFen();
}

Color Board::CurrentColor() const noexcept
{
    return current_player_color_;
}

Color Board::OpponentColor() const noexcept
{
    return current_player_color_ == Color::kWhite ? Color::kBlack : Color::kWhite;
}

Board::RookingFlags_t Board::RookingFlags() const noexcept
{
    return rooking_flags_;
}

size_t Board::TurnCounter() const noexcept
{
    return turn_counter_;
}

size_t Board::PassiveTurnCounter() const noexcept
{
    return passive_turn_counter_;
}

Position Board::LastPawnMove() const noexcept
{
    if(last_pawn_move_.Valid())
        if(current_player_color_ == Color::kWhite)
            return Position(last_pawn_move_.x()-1,last_pawn_move_.y());
        else
            return Position(last_pawn_move_.x()+1,last_pawn_move_.y());
    else
        return Position();
}

void Board::Set(Position position, Cell cell)
{
    assert(position.Valid());
    board_[position.Value()] = cell;
}

void Board::Swap(Position p1, Position p2)
{
    assert(p1.Valid());
    assert(p2.Valid());
    std::swap(board_[p1.Value()],board_[p2.Value()]);
}

void Board::SkipMove()
{
    current_player_color_ = OpponentColor();
}

bool Board::Test(Figure figure, Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].type == figure;
}

bool Board::TestColor(Color color, Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].color == color;
}

bool Board::TestEmp(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].type == Figure::kEmpty;
}

Board::Cell Board::GetCell(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()];
}

Figure Board::GetFigure(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].type;
}

Color Board::GetColor(Position position) const noexcept
{
    assert(position.Valid());
    return board_[position.Value()].color;
}

/*
Board::operator bool() const noexcept
{
    return TurnGenerate(*this,current_color).size() != 0;
}
*/

void Board::UnsafeExecuteTurn(Turn turn)
{
    if(last_pawn_move_.Valid() &&
            Test(Figure::kPawn,turn.from()) &&
            TestEmp(turn.to()) &&
            abs(last_pawn_move_.x() - turn.to().x())==1 &&
            abs(last_pawn_move_.y() - turn.from().y())==1 &&
            last_pawn_move_.y() == turn.to().y())
    {
        Set(last_pawn_move_,Cell{Figure::kEmpty,Color::kWhite});
    }
    //pawn special moves
    if(Test(Figure::kPawn,turn.from())&&abs(turn.from().x()-turn.to().x()) == 2)
        last_pawn_move_ = turn.to();
    else
        last_pawn_move_ = Position();
    //turn
    Set(turn.to(),{GetFigure(turn.from()),GetColor(turn.from())});
    if(turn.figure() != Figure::kEmpty)
        Set(turn.to(),{turn.figure(),GetColor(turn.from())});
    Set(turn.from(),{Figure::kEmpty,Color::kWhite});
    //rooking
    if(Test(Figure::kKing,turn.to())){
        if(turn.from() == Position(7,4) && turn.to() == Position(7,2) ){
            Swap(Position(7,0),Position(7,3));
        }
        else if(turn.from() == Position(7,4) && turn.to() == Position(7,6) ){
            Swap(Position(7,7),Position(7,5));
        }
        else if(turn.from() == Position(0,4) && turn.to() == Position(0,2) ){
            Swap(Position(0,0),Position(0,3));
        }
        else if(turn.from() == Position(0,4) && turn.to() == Position(0,6) ){
            Swap(Position(0,7),Position(0,5));
        }
    }
    //rooking disabling
    if(turn.from()== Position(7,0))
        rooking_flags_.white_ooo = false;
    else if( turn.from() == Position(7,7) )
        rooking_flags_.white_oo = false;
    else if(turn.from()== Position(0,0))
        rooking_flags_.black_ooo = false;
    else if( turn.from() == Position(0,7) )
        rooking_flags_.black_oo = false;
    else if( turn.from() == Position(7,4)){
        rooking_flags_.white_oo = false;
        rooking_flags_.white_ooo = false;
    }else if( turn.from() == Position(0,4)){
        rooking_flags_.black_oo = false;
        rooking_flags_.black_ooo = false;
    }else if(turn.to()== Position(7,0))
        rooking_flags_.white_ooo = false;
    else if( turn.to() == Position(7,7) )
        rooking_flags_.white_oo = false;
    else if(turn.to()== Position(0,0))
        rooking_flags_.black_ooo = false;
    else if( turn.to() == Position(0,7) )
        rooking_flags_.black_oo = false;

    /*
    turn_counter_++;
    if(TestEmp(turn.to()))
        passive_turn_counter_++;
    else
        passive_turn_counter_ = 0;
        */
    current_player_color_ = OpponentColor();
}

bool Board::ExecuteTurn(Turn turn)
{
    if(TestTurn(turn)==false)
        return false;

    UnsafeExecuteTurn(turn);
    return true;
}

bool Board::TestTurn(Turn turn) const
{
    auto turns = GenerateTurns();
    auto count = std::count(turns.begin(),turns.end(),turn);

    return count != 0;
}

/**
 * Константні зміщення ходів
 */
namespace
{
const int8_t knight_offsetx[8] = {2 , 1 , -2 , 1 , 2, -1 , -2 ,-1};
const int8_t knight_offsety[8] = {1 , 2 , 1 , -2, -1, 2 , -1 , -2};

const int8_t bishop_offsetx[4] = {1 , -1 , -1 , 1};
const int8_t bishop_offsety[4] = {1 , -1 , 1 , -1};

const int8_t rook_offsetx[4] = {1 , 0 , -1 , 0};
const int8_t rook_offsety[4] = {0 , 1 , 0 , -1};

const int8_t queen_king_offsetx[8] = {1 , -1 , -1 , 1 , 0 , 0 , 1 ,-1};
const int8_t queen_king_offsety[8] = {1 , -1 , 1 , -1 , 1 , -1, 0 , 0};


/**
 * @brief Шукає ходи для фігур (король і кінь)
 * @param[in] board Стартова дошка
 * @param[out] iterator Точка запису в кінцевий вектор
 * @param[in] x Початковий x
 * @param[in] y Початковий y
 * @param[in] offset_x вказівник на список зміщень по x
 * @param[in] offset_y вказівник на список зміщень по x
 * @param[in] size розмір списку
 */
void position_find(const Board &board, std::back_insert_iterator<std::vector<Turn>> &iterator,const uint8_t x,const uint8_t y, const int8_t offset_x[], const int8_t offset_y[], const size_t size)
{
    Color main_color = board.GetColor(Position(x,y)); ///< колір за який генеруємо ходи
    for(size_t i=0;i<size;i++)
    {
        uint8_t move_x = x + offset_x[i]; ///< задаємо x
        uint8_t move_y = y + offset_y[i]; ///< задаємо y

        if( !Position(move_x,move_y).Valid() ||
                (!board.TestEmp(Position(move_x,move_y)) && board.TestColor(main_color,Position(move_x,move_y))) ) ///< перевіряємо
            continue;
        *iterator = Turn({Position(x,y),Position(move_x,move_y)});
        ++iterator;
    }
}

/**
 * @brief Шукає ходи для фігур з рекурсією (офіцер, тура, королева)
 * @param[in] board Стартова дошка
 * @param[out] iterator Точка запису в кінцевий вектор
 * @param[in] x Початковий x
 * @param[in] y Початковий y
 * @param[in] offset_x вказівник на список зміщень по x
 * @param[in] offset_y вказівник на список зміщень по x
 * @param[in] size розмір списку
 */
void position_find_rec(const Board &board, std::back_insert_iterator<std::vector<Turn>> &iterator, uint8_t x, uint8_t y, const int8_t offset_x[], const int8_t offset_y[], const size_t size)
{
    bool main_color = (uint8_t)board.GetColor(Position(x,y)); ///< колір за який генеруємо ходи
    for(size_t i=0;i<size;i++){
        uint8_t move_x = x;
        uint8_t move_y = y;
        while(true){
            move_x += offset_x[i];
            move_y += offset_y[i];
            if( !Position(move_x,move_y).Valid() ||
                    (!board.TestEmp(Position(move_x,move_y)) && board.TestColor((Color)main_color,Position(move_x,move_y))) ) ///< тест на виліт з карти або стичку об свого
                break;

            *iterator = Turn({Position(x,y),Position(move_x,move_y)});
            ++iterator;

            if( !board.TestEmp(Position(move_x,move_y)) && board.TestColor((Color)!main_color,Position(move_x,move_y)) ) ///< тест на стичку з ворогом
                break;
        }
    }
}

/**
 * @brief Шукає ходи пішки
 * @param[in] board Стартова дошка
 * @param[out] iterator Точка запису в кінцевий вектор
 * @param[in] x Початковий x
 * @param[in] y Початковий y
 */
void position_find_pawn(const Board &board, std::back_insert_iterator<std::vector<Turn>> &iterator, uint8_t x, uint8_t y)
{
    auto push = [&iterator](Turn &&turn) ///< лямбда для доповнення списку
    {
        *iterator = turn;
        ++iterator;
    };
    auto pawn_move = [&push](Position p1 , Position p2) ///< лямбда для доповнення списку
    {
        push(Turn(p1 , p2 , Figure::kKnight));
        push(Turn(p1 , p2 , Figure::kBishop));
        push(Turn(p1 , p2 , Figure::kRook));
        push(Turn(p1 , p2 , Figure::kQueen));
    };
    Color main_color = board.GetColor(Position(x,y)); ///< колір за який генеруємо ходи

    if(main_color == Color::kWhite) ///< Генерація для білих
    {
        if(x > 1)
        {
            if( board.TestEmp(Position(x-1,y)) ) // хід
                push(Turn(Position(x,y) , Position(x-1,y)));
            if( board.TestEmp(Position(x-1,y)) && board.TestEmp(Position(x-2,y)) && x == 6) // двойний хід
                push(Turn(Position(x,y) , Position(x-2,y)));
            if( y != 0 && !board.TestEmp(Position(x-1,y-1)) && board.TestColor(Color::kBlack,Position(x-1,y-1)) ) // ліва атака
                push(Turn(Position(x,y) , Position(x-1,y-1)));
            if( y != 7 && !board.TestEmp(Position(x-1,y+1)) && board.TestColor(Color::kBlack,Position(x-1,y+1))) // права атака
                push(Turn(Position(x,y) , Position(x-1,y+1)));
        }
        else if( x == 1)  ///< ходи з перетворенням
        {
            if( board.TestEmp(Position(x-1,y)) ) // хід прямо
                pawn_move(Position(x,y) , Position(x-1,y));
            if( y!=0 && !board.TestEmp(Position(x-1,y-1)) && board.TestColor(Color::kBlack,Position(x-1,y-1))) // атака
                pawn_move(Position(x,y) , Position(x-1,y-1));
            if( y!=7 && !board.TestEmp(Position(x-1,y+1)) && board.TestColor(Color::kBlack,Position(x-1,y+1))) // атака
                pawn_move(Position(x,y) , Position(x-1,y+1));
        }
    } else
    {
        if(x < 6)  ///< звичайні ходи
        {
            if( board.TestEmp(Position(x+1,y)) ) // хід
                push(Turn({Position(x,y) , Position(x+1,y)}));
            if( board.TestEmp(Position(x+1,y)) && board.TestEmp(Position(x+2,y)) && x == 1) // двойний хід
                push(Turn({Position(x,y) , Position(x+2,y)}));
            if( y != 0 && !board.TestEmp(Position(x+1,y-1)) && board.TestColor(Color::kWhite,Position(x+1,y-1)) ) // ліва атака
                push(Turn({Position(x,y) , Position(x+1,y-1)}));
            if( y != 7 && !board.TestEmp(Position(x+1,y+1)) && board.TestColor(Color::kWhite,Position(x+1,y+1))) // права атака
                push(Turn({Position(x,y) , Position(x+1,y+1)}));
        }
        else if( x == 6) ///< ходи з перетворенням
        {
            if( board.TestEmp(Position(x+1,y)) ) // хід прямо
                pawn_move(Position(x,y) , Position(x+1,y));
            if( y!=0 && !board.TestEmp(Position(x+1,y-1)) && board.TestColor(Color::kWhite,Position(x+1,y-1))) // атака
                pawn_move(Position(x,y) , Position(x+1,y-1));
            if( y!=7 && !board.TestEmp(Position(x+1,y+1)) && board.TestColor(Color::kWhite,Position(x+1,y+1))) // атака
                pawn_move(Position(x,y) , Position(x+1,y+1));
        }
    }
}

void rooking_find(const Board &board, std::back_insert_iterator<std::vector<Turn>> &iterator)
{
    auto push = [&iterator](Turn &&turn) ///< лямбда для доповнення списку
    {
        *iterator = turn;
        ++iterator;
    };
    if(board.CurrentColor() == Color::kWhite){
        if( board.RookingFlags().white_ooo && board.TestEmp(Position(7,1)) && board.TestEmp(Position(7,2)) && board.TestEmp(Position(7,3))) // ліва рокіровка
            push(Turn(Position(7,4),Position(7,2)));
        if( board.RookingFlags().white_oo  && board.TestEmp(Position(7,5)) &&  board.TestEmp(Position(7,6))) // права рокіровка
            push(Turn(Position(7,4),Position(7,6)));
    } else
    {
        if( board.RookingFlags().black_ooo && board.TestEmp(Position(0,1)) && board.TestEmp(Position(0,2)) && board.TestEmp(Position(0,3))) // ліва рокіровка
            push(Turn(Position(0,4),Position(0,2)));
        if( board.RookingFlags().black_oo  && board.TestEmp(Position(0,5)) &&  board.TestEmp(Position(0,6))) // права рокіровка
            push(Turn(Position(0,4),Position(0,6)));
    }
}

void empasant_find(const Board &board,Position last_pawn_move, std::back_insert_iterator<std::vector<Turn>> &iterator)
{
    auto push = [&iterator](Turn &&turn) ///< лямбда для доповнення списку
    {
        *iterator = turn;
        ++iterator;
    };
    if(last_pawn_move.Valid())
    {
        if(board.CurrentColor() == Color::kWhite)
        {
            if( last_pawn_move.y() != 0 && board.TestColor(Color::kWhite,Position(last_pawn_move.x(),last_pawn_move.y()-1)) &&
                    board.Test(Figure::kPawn,Position(last_pawn_move.x(),last_pawn_move.y()-1))) // ліва атака
                push(Turn({Position(last_pawn_move.x(),last_pawn_move.y()-1) , Position(last_pawn_move.x()-1,last_pawn_move.y())}));
            if( last_pawn_move.y() != 7 && board.TestColor(Color::kWhite,Position(last_pawn_move.x(),last_pawn_move.y()+1)) &&
                    board.Test(Figure::kPawn,Position(last_pawn_move.x(),last_pawn_move.y()+1))) // ліва атака
                push(Turn({Position(last_pawn_move.x(),last_pawn_move.y()+1) , Position(last_pawn_move.x()-1,last_pawn_move.y())}));
        } else {
            if( last_pawn_move.y() != 0 && board.TestColor(Color::kBlack,Position(last_pawn_move.x(),last_pawn_move.y()-1)) &&
                    board.Test(Figure::kPawn,Position(last_pawn_move.x(),last_pawn_move.y()-1))) // ліва атака
                push(Turn({Position(last_pawn_move.x(),last_pawn_move.y()-1) , Position(last_pawn_move.x()+1,last_pawn_move.y())}));
            if( last_pawn_move.y() != 7 && board.TestColor(Color::kBlack,Position(last_pawn_move.x(),last_pawn_move.y()+1)) &&
                    board.Test(Figure::kPawn,Position(last_pawn_move.x(),last_pawn_move.y()+1))) // ліва атака
                push(Turn({Position(last_pawn_move.x(),last_pawn_move.y()+1) , Position(last_pawn_move.x()+1,last_pawn_move.y())}));
        }
    }
}
}

/**
 * @brief Генерує сухий набір ходів без врахування мату
 * @param board Дошка
 * @param color Колір за який проводиться рахунок
 * @return Вектор ходів
 */
std::vector<Turn> Board::UnsafeTurns(Color color) const
{
    std::vector<Turn> turns;turns.reserve(60);
    auto inserter = std::back_inserter(turns);

    for(uint8_t x=0;x<8;x++)
    {
        for(uint8_t y=0;y<8;y++){

            if(TestEmp(Position(x,y)) || !TestColor(color,Position(x,y))) ///< Пропуск пустих і ворожих клітинок
                continue;

            switch (GetFigure(Position(x,y))) {

            case Figure::kEmpty:
                break;

            case Figure::kPawn:
                position_find_pawn(*this,inserter,x,y);
                break;

            case Figure::kKnight:
                position_find(*this,inserter,x,y,knight_offsetx,knight_offsety,8);
                break;

            case Figure::kBishop:
                position_find_rec(*this,inserter,x,y,bishop_offsetx,bishop_offsety,4);
                break;

            case Figure::kRook:
                position_find_rec(*this,inserter,x,y,rook_offsetx,rook_offsety,4);
                break;

            case Figure::kQueen:
                position_find_rec(*this,inserter,x,y,queen_king_offsetx,queen_king_offsety,8);
                break;

            case Figure::kKing:
                position_find(*this,inserter,x,y,queen_king_offsetx,queen_king_offsety,8);
                break;
            }
        }
    }
    rooking_find(*this,inserter);
    empasant_find(*this,last_pawn_move_,inserter);

    return turns;
}

std::vector<Turn> Board::GenerateTurns() const
{
    return GenerateTurns(current_player_color_);
}

std::vector<Board> Board::GenerateSubBoards() const
{
    std::vector<Board> boards;
    auto turns = GenerateTurns();
    for(auto turn : turns)
    {
        Board board(*this);
        board.ExecuteTurn(turn);
        boards.push_back(board);
    }
    return boards;
}
std::vector<Turn> Board::GenerateTurns(Color color) const
{
    /*
    auto atack_on = [](Turn turn , Position pos)
    {
    return turn.to() == pos;
    };
    */
    auto is_mat = [this](Turn turn) -> bool
    {
        auto find_king = [](Board b , Color color)
        {
            for(uint8_t i = 0 ; i < 64 ; i++)
            {
                if(b.GetFigure(i) == Figure::kKing && b.GetColor(i) == color)
                    return Position(i);
            }
            return Position();
        };
        Board copy(*this);
        copy.UnsafeExecuteTurn(turn);
        auto turns = copy.UnsafeTurns(copy.CurrentColor());
        auto king_pos = find_king(copy,CurrentColor());
        bool flag = false;
        for(auto turn : turns){
            if(turn.to() == king_pos){
                flag = true;
                break;
            }
        }
        return flag;
    };
    auto delete_mate_add = [this](Turn turn) {
        if(Test(Figure::kKing,turn.from()) && MateTest() &&
                ( abs(turn.from().y() - turn.to().y()) == 2) )
        {
            return true;
        }
        return false;
    };

    auto rooking_lock = [this](Turn turn) {
        if(Test(Figure::kKing,turn.from()) &&
                ( turn.from().y() != 0 && turn.from().y() - turn.to().y() == 2) &&
                UnderAtack(Position(turn.from().x(),turn.from().y()-1))
                )
        {
            return true;
        }
        if(Test(Figure::kKing,turn.from()) &&
                ( turn.from().y() != 7 && turn.from().y() - turn.to().y() == -2) &&
                UnderAtack(Position(turn.from().x(),turn.from().y()+1))
                )
        {
            return true;
        }
        return false;
    };

    auto turns = UnsafeTurns(current_player_color_);

    auto it = std::remove_if(turns.begin(),turns.end(),is_mat);
    turns.erase(it,turns.end());
    it = std::remove_if(turns.begin(),turns.end(),delete_mate_add);
    turns.erase(it,turns.end());
    it = std::remove_if(turns.begin(),turns.end(),rooking_lock);
    turns.erase(it,turns.end());

    return turns;
}

bool Board::MateTest() const {
    auto find_king = [](Board b , Color color)
    {
        for(uint8_t i = 0 ; i < 64 ; i++)
        {
            if(b.GetFigure(i) == Figure::kKing && b.GetColor(i) == color)
                return Position(i);
        }
        return Position();
    };
    auto king_pos = find_king(*this,CurrentColor());
    return UnderAtack(king_pos);
}


bool Board::UnderAtack(Position position) const
{
    assert(position.Valid());
    auto turns = UnsafeTurns(OpponentColor());
    for(auto turn : turns){
        if(turn.to() == position){
            return true;
        }
    }
    return false;
}
