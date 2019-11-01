#include "Chess.h"
#include <array>

namespace Chess{
	
	class ChessController;
	class AbstractBot;

	class Chessboard
	{
		public:

			bool whiteTurn = true; // чий хід
            bool LeftRooking = true; // true - можливо зробити рокіровку false - неможливо
            bool RightRooking = true;

            std::array<Figure,64> map;
			
            bool makeTurn(Turn turn , bool SafeMode);
            bool undoTurn();
			
			
			Chessboard();
			Chessboard(const Chessboard&); // конструктор копіювання
				
			bool getCurrentTurns(std::vector<Turn> &turns);

            Figure& at(unsigned x , unsigned y)
            {
                return (this->map[(x * 8)+y]);
            }

            Figure& at(Position p)
            {
                return (this->map[(p.x * 8)+p.y]);
            }

			Chessboard& operator= (const Chessboard &str); // оператор копіювання
			Chessboard operator ~() const;
		
		
	};
	
}

