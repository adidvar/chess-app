#include "MyBot.h"
#define INFINITY 99999.0

namespace Chess{

float MyBot::GetPositionMark(Chessboard &board)
{
    std::array<std::array<int,64>,7> posprice = {
        std::array<int,64>
        {0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0,
         0,0,0,0,0,0,0,0},
        std::array<int,64>
        {0,0,0,0,0,0,0,0,
         5,5,5,5,5,5,5,5,
         1,1,2,3,3,2,1,1,
         1,1,1,3,3,1,1,1,
         0,0,0,2,2,0,0,0,
         1,-1,-1,0,0,-1,-1,1,
         1,1,1,-2,-2,1,1,1,
         0,0,0,0,0,0,0,0},
        std::array<int,64>
        {-5,-4,-3,-3,-3,-3,-4,-5,
         -4,-2,0,0,0,0,-2,-4,
         -3,0,0,0,0,0,0,-3,
         -3,0,0,0,0,0,0,-3,
         -3,0,0,0,0,0,0,-3,
         -3,0,0,0,0,0,0,-3,
         -4,-2,0,0,0,0,-2,-4,
         -5,-3,-3,-3,-3,-3,-4,-5},
        std::array<int,64>
        {-2,-1,-1,-1,-1,-1,-1,-2,
         -1,0,0,0,0,0,0,-1,
         -1,0,1,1,1,1,0,-1,
         -1,1,1,1,1,1,1,-1,
         -1,0,1,1,1,1,0,-1,
         -1,1,1,1,1,1,1,-1,
         -1,1,0,0,0,0,1,-1,
         -2,-1,-1,-1,-1,-1,-1,-2},
        std::array<int,64>
        {0,0,0,0,0,0,0,0,
         1,1,1,1,1,1,1,1,
         -1,0,0,0,0,0,0,-1,
         -1,0,0,0,0,0,0,-1,
         -1,0,0,0,0,0,0,-1,
         -1,0,0,0,0,0,0,-1,
         -1,0,0,0,0,0,0,-1,
         0,0,0,1,1,0,0,0},
        std::array<int,64>
        {-2,-1,-1,-1,-1,-1,-1,-2,
         -1,0,0,0,0,0,0,-1,
         -1,1,1,1,1,1,1,-1,
         -1,1,1,1,1,1,1,-1,
         1,1,1,1,1,1,1,-1,
         -1,1,1,1,1,1,1,-1,
         -1,0,1,0,0,0,0,-1,
         -2,-1,-1,-1,-1,-1,-1,-2},
         std::array<int,64>
        {1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1,
         1,1,1,1,1,1,1,1}
    };

    std::array<int,7> price {0,10,30,30,50,90,900};
    int PositionMark = 0;
    for(int i=0;i<64;i++)
    {
        Chess::FigureType type = board.map[i].type;
        PositionMark += ((board.map[i].color == Chess::Color::White)
                ? (price.at(type) +  posprice.at(type).at(i))
                : -(price.at(type) + posprice.at(type).at(i)));
    }
    return (float)PositionMark;
}
	
    float MyBot::GetTreeMark(Chessboard &board , bool MinMax , unsigned TTL )
	{
		 //якщо кінчилась глибина
		 if(TTL<=0){
            return GetPositionMark(board);
		 }
		 //якщо глубина не закінчилася
		 std::vector<Turn> turns;
		 board.getCurrentTurns(turns);
		 
		 std::vector<float> marks;

		 for ( Turn turn : turns) {
			 Chessboard newboard(board);
			 newboard.makeTurn(turn,false);
			 newboard = ~newboard;
             float mark = GetTreeMark(newboard ,!MinMax , TTL-1);
             marks.push_back(mark);
				
		 }
		 
		 float min = INFINITY, max = -INFINITY;
		 
		 if(!marks.empty()){
			 min = marks.front();
			 max = marks.front();
			 
			 for ( float mark : marks) {
				 if(mark > max)
					 max = mark;
				 if(mark < min)
					 min = mark;
			}
            return (MinMax ? max : min);
		 }
            return (MinMax ? -INFINITY : INFINITY);
	}

    bool MyBot::GetTurn(Turn &t)
	{
		 std::vector<Turn> turns;
		 board.getCurrentTurns(turns);

         std::vector<float> marks;

         std::vector<Turn>::iterator it = turns.begin();
         for ( ;it != turns.end();it++) {
             Chessboard newboard(board);
             newboard.makeTurn(*it,false);
             newboard = ~newboard;
             float mark = GetTreeMark(newboard , false , (int)mode);
             marks.push_back(mark);
         }
		 if(!marks.empty()){
			 
         float max;
		 unsigned maxindex;
			 
            max = marks.front();
            maxindex = 0;
			int i = 0;
             for ( float mark : marks) {
                 if(mark > max){
                     max = mark;
                     maxindex = i;
                 }
                 i++;
             }

             t = turns[maxindex];
             return true;

		 }
            return true;
    }
};
