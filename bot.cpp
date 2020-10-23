#include "bot.h"

const int16_t max_value = std::numeric_limits<int16_t>::max();

int16_t Bot::Mark(Board &board)
{
    std::array<int,7> price {0,10,30,30,50,90,900};
    int16_t mark = 0;
    for(uint8_t i=0;i<64;i++)
    {
        Figures type = board.GetFigure(i);
        mark += ((board.TestColor(white,i))
                 ? price.at(type)
                 : -price.at(type));

    }
    return mark;
}

int16_t Bot::MarkTreeBranch(Board board , unsigned depth , int16_t alpha , int16_t beta )
{
    if(depth<=0)
        return Mark(board);

    //якщо глубина не закінчилася

    int16_t max = -max_value;

    for (TurnGenerator generator(board);generator;++generator)
    {
        Board sub_board(board);

        TurnExecutor::ExecuteTurn(sub_board,*generator);

        int16_t mark = -MarkTreeBranch(sub_board,depth-1 , -beta , -alpha);

        if(mark >= beta)
            return beta;

        if(mark > alpha)
            alpha = mark;


    }
    return alpha;
}

Turn Bot::GetTurn()
{
    int16_t max = -max_value;
    Turn turn;

    for (TurnGenerator generator(board);generator;++generator)
    {
        Board sub_board(board);

        TurnExecutor::ExecuteTurn(sub_board,*generator);

        int16_t mark = -MarkTreeBranch(sub_board,this->depth , -max_value , max_value);

        if(mark > max)
        {
            max = mark;
            turn = *generator;
        }
    }

    return turn;
}
