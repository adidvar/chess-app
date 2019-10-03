#include "MyBot.h" 
#define INFINITY 9999999999.0

namespace Chess{

void MyBot::GetPositionMark(float &PositionMark , Chessboard &board)
{
    std::array<std::array<float,64>,7> posprice = {
        std::array<float,64>
        {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
        std::array<float,64>
        {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,5.0f,
         1.0f,1.0f,2.0f,3.0f,3.0f,2.0f,1.0f,1.0f,
         0.5f,0.5f,1.0f,2.5f,2.5f,1.0f,0.5f,0.5f,
         0.0f,0.0f,0.0f,2.0f,2.0f,0.0f,0.0f,0.0f,
         0.5f,-0.5f,-1.0f,0.0f,0.0f,-1.0f,-0.5f,0.5f,
         0.5f,1.0f,1.0f,-2.0f,-2.0f,1.0f,1.0f,0.5f,
         0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
        std::array<float,64>
        {-5.0f,-4.0f,-3.0f,-3.0f,-3.0f,-3.0f,-4.0f,-5.0f,
         -4.0f,-2.0f,0.0f,0.0f,0.0f,0.0f,-2.0f,-4.0f,
         -3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-3.0f,
         -3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-3.0f,
         -3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-3.0f,
         -3.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-3.0f,
         -4.0f,-2.0f,0.0f,0.0f,0.0f,0.0f,-2.0f,-4.0f,
         -5.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-4.0f,-5.0f},
        std::array<float,64>
        {-2.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-2.0f,
         -1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,
         -1.0f,0.0f,0.5f,1.0f,1.0f,0.5f,0.0f,-1.0f,
         -1.0f,0.5f,0.5f,1.0f,1.0f,0.5f,0.5f,-1.0f,
         -1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,-1.0f,
         -1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,
         -1.0f,0.5f,0.0f,0.0f,0.0f,0.0f,0.5f,-1.0f,
         -2.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-2.0f},
        std::array<float,64>
        {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,
         0.5f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.5f,
         -0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.5f,
         -0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.5f,
         -0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.5f,
         -0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.5f,
         -0.5f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-0.5f,
         0.0f,0.0f,0.0f,0.5f,0.5f,0.0f,0.0f,0.0f},
        std::array<float,64>
        {-2.0f,-1.0f,-1.0f,-0.5f,-0.5f,-1.0f,-1.0f,-2.0f,
         -1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,
         -1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,
         -1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-0.5f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-0.5f,
         -1.0f,0.5f,0.5f,0.5f,0.5f,0.5f,1.0f,-1.0f,
         -1.0f,0.0f,0.5f,0.0f,0.0f,0.0f,0.0f,-1.0f,
         -2.0f,-1.0f,-1.0f,-0.5f,-0.5f,-1.0f,-1.0f,-2.0f},
		 std::array<float,64>
        {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
         1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f}
    };





    std::array<int,7> price {0,10,30,30,50,90,900};
    PositionMark = 0;
    for(int i=0;i<64;i++)
    {
        PositionMark += ((board.map[i].color == Chess::Color::White)
                ? (price.at(board.map[i].type) +  posprice.at(board.map[i].type).at(i))
                : -(price.at(board.map[i].type) + posprice.at(board.map[i].type).at(i)));
    }
}
	
    void MyBot::GetTreeMark(float &TreeMark , bool &TreeResult , Chessboard &board , bool MinMax , unsigned TTL )
	{
		 //якщо кінчилась глибина
		 if(TTL<=0){
             GetPositionMark(TreeMark , board);
			 TreeResult = true;
			 return;
		 }
		 //якщо глубина не закінчилася
		 std::vector<Turn> turns;
		 board.getCurrentTurns(turns);
		 
		 std::vector<float> marks;

		 for ( Turn turn : turns) {
			 Chessboard newboard(board);
			 newboard.makeTurn(turn,false);
			 newboard = ~newboard;
			 float mark = 0;
			 bool result = false;
			 GetTreeMark(mark, result, newboard ,!MinMax , TTL-1);
			 if(result == true)
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
			TreeMark = (MinMax ? max : min);
			TreeResult = true;
			return;	
		 }
		 
		 TreeResult = false;
			return;
	}

	bool MyBot::GetTurn(Turn &t,Chessboard &board)
	{
		 std::vector<Turn> turns;
		 board.getCurrentTurns(turns);

         std::vector<float> marks;

         std::vector<Turn>::iterator it = turns.begin();
         for ( ;it != turns.end();it++) {
             Chessboard newboard(board);
             newboard.makeTurn(*it,false);
             newboard = ~newboard;
             float mark;
             bool result;
             GetTreeMark(mark, result, newboard , true , 4);
             if(true) //***
                marks.push_back(mark);
		 }
		 if(!marks.empty()){
			 
			 float max;
		 unsigned maxindex;
			 
			 max = marks.front();
			 maxindex = 0;
			int i = 0;
             for ( float mark : marks) {
                 if(mark < max){
                     max = mark;
                     maxindex = i;
                 }
                 i++;
             }
             t = turns[maxindex];
             return true;

		 }
            return false;
    }
};
