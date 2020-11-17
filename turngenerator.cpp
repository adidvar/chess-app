
#include "turngenerator.h"

enum prior
{
    pawn_move = 1,
    knight_move = 2,
    bishop_move = 3,
    rook_move = 4,
    queue_move = 5,
    king_move = 6,
    pawn_atack = 7,
    knight_atack = 8,
    king_atack = 9,
    bishop_atack = 10,
    rook_atack = 11,
    queue_atack = 12,
    pawn_lasthit = 13,
    pawn_lasthit_with_atack = 14
};

bool operator < (TurnWrapper w1 , TurnWrapper w2)
{
    return w1.priority < w2.priority;
}

void TurnGenerator::position_execute(const int8_t offset_x[], const int8_t offset_y[], size_t size , uint8_t x , uint8_t y ,  int8_t priority_move, int8_t priority_atack)
{
    uint8_t move_x = 0;
    uint8_t move_y = 0;
    for(size_t i=0;i<size;i++)
    {
        move_x = x + offset_x[i]; // задаємо x
        move_y = y + offset_y[i]; // задаємо y

        if( !PositionNormal( Position(move_x,move_y) ) ||
                (!board.TestEmp(Position(move_x,move_y)) && board.TestColor(white,Position(move_x,move_y))) ) // перевіряємо
            continue;
        if (board.TestEmp(Position(move_x,move_y)))
            turns.push(TurnWrapper(Turn(Position(x,y),Position(move_x,move_y)),priority_move));
        else
            turns.push(TurnWrapper(Turn(Position(x,y),Position(move_x,move_y)),priority_atack));
    }
}

void TurnGenerator::positon_execute_resursive(const int8_t offset_x[], const int8_t offset_y[], size_t size, uint8_t x, uint8_t y, int8_t priority_move, int8_t priority_atack)
{
    uint8_t move_x = 0;
    uint8_t move_y = 0;
    for(size_t i=0;i<size;i++){
        move_x = x;
        move_y = y;
        while(true){
            move_x += offset_x[i];
            move_y += offset_y[i];
            if( !PositionNormal( Position(move_x,move_y) ) ||
                    (!board.TestEmp(Position(move_x,move_y)) && board.TestColor(white,Position(move_x,move_y))) ) // тест на виліт з карти або стичку об свого
                break;

            if( !board.TestEmp(Position(move_x,move_y)) && board.TestColor(black,Position(move_x,move_y)) )
            { // тест на стичку з ворогом
                turns.push(TurnWrapper(Turn(Position(x,y),Position(move_x,move_y)),priority_atack));
                break;
            }

            turns.push(TurnWrapper(Turn(Position(x,y),Position(move_x,move_y)),priority_move));

        }
    }
}

namespace
{
const int8_t knight_offsetx[8] = {2 , 1 , -2 , 1 , 2, -1 , -2 ,-1};
const int8_t knight_offsety[8] = {1 , 2 , 1 , -2, -1, 2 , -1 , -2};

const int8_t bishop_offsetx[4] = {1 , -1 , -1 , 1};
const int8_t bishop_offsety[4] = {1 , -1 , 1 , -1};

const int8_t rook_offsetx[4] = {1 , 0 , -1 , 0};
const int8_t rook_offsety[4] = {0 , 1 , 0 , -1};

const int8_t queen_offsetx[8] = {1 , -1 , -1 , 1 , 0 , 0 , 1 ,-1};
const int8_t queen_offsety[8] = {1 , -1 , 1 , -1 , 1 , -1, 0 , 0};

const int8_t king_offsetx[8] = {1 , -1 , -1 , 1 , 0 , 0 , 1 ,-1};
const int8_t king_offsety[8] = {1 , -1 , 1 , -1 , 1 , -1, 0 , 0};

}

TurnGenerator::TurnGenerator(const Board &board):
    board(board)
{

    for(uint8_t x=0;x<8;x++)
    {
        for(uint8_t y=0;y<8;y++){

            if(board.TestEmp(Position(x,y)) || board.TestColor(black,Position(x,y)))//пуста клітинка
                continue;

            switch ((Figures_enum)board.GetFigure(Position(x,y))) {

            case empty:
                break;

            case pawn:

                if(x > 1)
                {
                    if( board.TestEmp(Position(x-1,y)) ) // хід
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y)),pawn_move));
                    if( board.TestEmp(Position(x-1,y)) && board.TestEmp(Position(x-2,y)) && x == 6) // двойний хід
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-2,y)),pawn_move));
                    if( y != 0 && !board.TestEmp(Position(x-1,y-1)) && board.TestColor(black,Position(x-1,y-1)) ) // ліва атака
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y-1)),pawn_atack));
                    if( y != 7 && !board.TestEmp(Position(x-1,y+1)) && board.TestColor(black,Position(x-1,y+1))) // права атака
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y+1)),pawn_atack));
                }
                else if( x == 1)
                { // якщо це хід з перетворенням
                    if( board.TestEmp(Position(x-1,y)) ) // хід прямо
                    {
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y) , knight),pawn_lasthit));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y) , bishop),pawn_lasthit));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y) , rook),pawn_lasthit));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y) , queen),pawn_lasthit));
                    }
                    if( y!=0 && !board.TestEmp(Position(x-1,y-1)) && board.TestColor(black,Position(x-1,y-1))) // атака
                    {
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y-1) , knight),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y-1) , bishop),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y-1) , rook),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y-1) , queen),pawn_lasthit_with_atack));
                    }
                    if( y!=7 && !board.TestEmp(Position(x-1,y+1)) && board.TestColor(black,Position(x-1,y+1))) // атака
                    {
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y+1) , knight),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y+1) , bishop),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y+1) , rook),pawn_lasthit_with_atack));
                        turns.push(TurnWrapper(Turn(Position(x,y) , Position(x-1,y+1) , queen),pawn_lasthit_with_atack));
                    }

                }

                break;

            case knight:
                position_execute(knight_offsetx,knight_offsety,8,x,y , knight_move , knight_atack);
                break;

            case bishop:
                positon_execute_resursive(bishop_offsetx,bishop_offsety,4,x,y , bishop_move , bishop_atack);
                break;

            case rook:
                positon_execute_resursive(rook_offsetx,rook_offsety,4,x,y , rook_move , rook_atack);
                break;

            case queen:
                positon_execute_resursive(queen_offsetx,queen_offsety,8,x,y , queue_move , queue_atack);
                break;

            case king:
                position_execute(king_offsetx,king_offsety,8,x,y , king_move , king_atack);
                break;
            }
        }
    }

    //    if(whiteTurn == true){
    //        if(rooking[whiteTurn].LeftRooking == true && at(7,1).type == Emply && at(7,2).type == Emply && at(7,3).type == Emply) // ліва рокіровка
    //            turns.push_back(Turn(true));
    //        if(rooking[whiteTurn].RightRooking == true && at(7,5).type == Emply && at(7,6).type == Emply) // права рокіровка
    //            turns.push_back(Turn(false));
    //    } else
    //    {
    //        if(rooking[!whiteTurn].LeftRooking == true && at(7,1).type == Emply && at(7,2).type == Emply) // ліва рокіровка
    //            turns.push_back(Turn(true));
    //        if(rooking[!whiteTurn].RightRooking == true && at(7,5).type == Emply && at(7,5).type == Emply && at(7,6).type == Emply) // права рокіровка
    //            turns.push_back(Turn(false));
    //    }
}

Turn TurnGenerator::operator *() const
{
    return turns.top().turn;
}

void TurnGenerator::operator ++()
{
    turns.pop();
}

TurnGenerator::operator bool() const
{
    return !turns.empty();
}
