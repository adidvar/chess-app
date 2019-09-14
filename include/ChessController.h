#include "AbstractBot.h"


namespace Chess{
	
	class ChessController
	{
		
		public:
            ChessController(AbstractBot &bp , AbstractBot &wp):
            black_player(bp),
            white_player(wp)
            {};
			Chessboard GetGlobalBoard();
			Chessboard GetLocalBoard();
            bool NextMove();
			Chessboard board;
			AbstractBot &black_player;
			AbstractBot &white_player;
		
	};
}

