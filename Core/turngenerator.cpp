#include "board.h"

#include <variant>
#include <tuple>
#include <algorithm>
#include <iterator>

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

}

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
            if( y != 0 && !board.TestEmp(Position(x+1,y-1)) && board.TestColor(Color::kBlack,Position(x+1,y-1)) ) // ліва атака
                push(Turn({Position(x,y) , Position(x+1,y-1)}));
            if( y != 7 && !board.TestEmp(Position(x+1,y+1)) && board.TestColor(Color::kBlack,Position(x+1,y+1))) // права атака
                push(Turn({Position(x,y) , Position(x+1,y+1)}));
        }
        else if( x == 6) ///< ходи з перетворенням
        {
            if( board.TestEmp(Position(x+1,y)) ) // хід прямо
                pawn_move(Position(x,y) , Position(x+1,y));
            if( y!=0 && !board.TestEmp(Position(x+1,y-1)) && board.TestColor(Color::kBlack,Position(x+1,y-1))) // атака
                pawn_move(Position(x,y) , Position(x+1,y-1));
            if( y!=7 && !board.TestEmp(Position(x+1,y+1)) && board.TestColor(Color::kBlack,Position(x+1,y+1))) // атака
                pawn_move(Position(x,y) , Position(x+1,y+1));
        }
    }
}

/*
void rooking_find(const Board &board, std::back_insert_iterator<std::vector<Position>> &iterator)
{
    auto push = [&iterator](Position &&turn) ///< лямбда для доповнення списку
    {
        *iterator = turn;
        ++iterator;
    };
    if(board.CurrentColor() == Color::kWhite){
        if( board.RookingFlags().white_ooo && board.TestEmp(Position(7,1)) && board.TestEmp(Position(7,2)) && board.TestEmp(Position(7,3))) // ліва рокіровка
            push(Position(rooking_ooo));
        if( board.RookingState().white_oo  && board.TestEmp(Position(7,5)) &&  board.TestEmp(Position(7,6))) // права рокіровка
            push(Position(rooking_oo));
    } else
    {
        if(board.RookingFlags().black_ooo && board.TestEmp(Position(0,1)) && board.TestEmp(Position(0,2)) && board.TestEmp(Position(0,3)) ) // ліва рокіровка
            push(Position(rooking_ooo));
        if(board.RookingState().white_oo && board.TestEmp(Position(0,5)) &&  board.TestEmp(Position(0,6)) ) // права рокіровка
            push(Position(rooking_oo));
    }
}
*/

/**
 * @brief Генерує сухий набір ходів без врахування мату
 * @param board Дошка
 * @param color Колір за який проводиться рахунок
 * @return Вектор ходів
 */
std::vector<Turn> UnsafeGenerator(Board board , Color color)
{
    std::vector<Turn> turns;turns.reserve(60);
    auto inserter = std::back_inserter(turns);

    for(uint8_t x=0;x<8;x++)
    {
        for(uint8_t y=0;y<8;y++){

            if(board.TestEmp(Position(x,y)) || !board.TestColor(color,Position(x,y))) ///< Пропуск пустих і ворожих клітинок
                continue;

            switch (board.GetFigure(Position(x,y))) {

            case Figure::kEmpty:
                break;

            case Figure::kPawn:
                position_find_pawn(board,inserter,x,y);
                break;

            case Figure::kKnight:
                position_find(board,inserter,x,y,knight_offsetx,knight_offsety,8);
                break;

            case Figure::kBishop:
                position_find_rec(board,inserter,x,y,bishop_offsetx,bishop_offsety,4);
                break;

            case Figure::kRook:
                position_find_rec(board,inserter,x,y,rook_offsetx,rook_offsety,4);
                break;

            case Figure::kQueen:
                position_find_rec(board,inserter,x,y,queen_king_offsetx,queen_king_offsety,8);
                break;

            case Figure::kKing:
                position_find(board,inserter,x,y,queen_king_offsetx,queen_king_offsety,8);
                break;
            }
        }
    }
 //   rooking_find(board,inserter);
    return turns;
}

std::vector<Turn> Board::GenerateTurns() const
{
    auto atack_on = [](Turn turn , Position pos)
    {
        return turn.to() == pos;
    };

    auto find_king = [](Board b , Color color)
    {
        for(uint8_t i = 0 ; i < 64 ; i++)
        {
            if(b.GetFigure(i) == Figure::kKing && b.GetColor(i) == color)
                return Position(i);
        }
        return Position();
    };

    auto is_mat = [this,atack_on , find_king](Turn turn)
    {
        auto board_copy = *this;
        board_copy.ExecuteTurn(turn);

        auto turns = UnsafeGenerator(board_copy,(Color)!(int)current_player_color_);
        auto king_pos = find_king(board_copy,current_player_color_);

        for(auto &t : turns)
        {
            if(atack_on(t,king_pos))
                return true;
        }
        return false;
    };

        auto turns = UnsafeGenerator(*this,current_player_color_);

    auto it = std::remove_if(turns.begin(),turns.end(),is_mat);
    turns.erase(it,turns.end());

    return turns;
}



