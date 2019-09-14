#include "MyBot.h"
#include <random>
#include <iostream>

namespace Chess{

	void MyBot::GetPositionMark(double &PositionMark , Chessboard &board)
	{
		PositionMark = 0;
		for(int i=0;i<64;i++)
		{
			PositionMark += (board.map[i].color == Chess::Color::White) ? (board.map[i].type) : -(board.map[i].type);
		}
	}
	
	void MyBot::GetTreeMark(double &TreeMark , bool &TreeResult , Chessboard &board , bool MinMax , unsigned TTL )
	{
		 //якщо кінчилась глибина
		 if(TTL==0){
			 GetPositionMark(TreeMark , board);
			 TreeMark  = MinMax ? -TreeMark : TreeMark;
			 TreeResult = true;
			 return;
		 }
		 //якщо глубина не закінчилася
		 std::vector<Turn> turns;
		 board.getCurrentTurns(turns);
		 
		 std::vector<double> marks;

		 for ( Turn turn : turns) {
			 Chessboard newboard(board);
			 newboard.makeTurn(turn,false);
			 newboard = ~newboard;
			 double mark = 0;
			 bool result = false;
			 GetTreeMark(mark, result, newboard ,!MinMax , TTL-1);
			 if(result == true)
				marks.push_back(mark);
				
		 }
		 
		 double min = 0 , max = 0;
		 
		 if(!marks.empty()){
			 min = marks.front();
			 max = marks.front();
			 
			 for ( double mark : marks) {
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

		 std::vector<double> marks;

         std::vector<Turn>::iterator it = turns.begin();
         for ( ;it != turns.end();it++) {
             Chessboard newboard(board);
             newboard.makeTurn(*it,false);
             newboard = ~newboard;
             double mark;
             bool result;
             GetTreeMark(mark, result, newboard , true , 2);
             if(true) //***
                marks.push_back(mark);
			 std::cout << mark << std::endl;


		 }
		 
		 double max;
		 unsigned maxindex = 0 , i = 0;
		 
		 if(!marks.empty()){
			 max = marks.front();
			 maxindex = 0;

             for ( double mark : marks) {
                 if(mark > max){
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
