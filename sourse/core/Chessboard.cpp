#include "Chessboard.h"

namespace Chess{

	Chessboard::Chessboard()
	{
		Figure start[64] = {
            Figure(Rook,Black),Figure(Knight,Black),Figure(Bishop,Black),Figure(King,Black),Figure(Queen,Black),Figure(Bishop,Black),Figure(Knight,Black),Figure(Rook,Black),
			Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),Figure(Pawn,Black),
			Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),
			Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),Figure(Emply,Black),
			Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),
			Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),Figure(Emply,White),
			Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),Figure(Pawn,White),
            Figure(Rook,White),Figure(Knight,White),Figure(Bishop,White),Figure(King,White),Figure(Queen,White),Figure(Bishop,White),Figure(Knight,White),Figure(Rook,White),
		};
        for(unsigned i=0;i< 64 ; i++)
		{
			map[i] = start[i]; 
		}
	}
	

	Chessboard::Chessboard(const Chessboard& reference) 
	{
        for(unsigned i=0;i<64;i++)
		{
				map[i] = reference.map[i];
		}
		this->whiteTurn = reference.whiteTurn; 
	}
	
	Chessboard Chessboard::operator ~() const
	{
		Chessboard revercecopy(*this);
		Figure temp;
        for(unsigned i=0;i<32;i++)
		{
			Figure temp = revercecopy.map[i];
			revercecopy.map[i] = revercecopy.map[63-i];
			revercecopy.map[63-i] = temp;
			revercecopy.map[i].color = revercecopy.map[i].color ? White : Black;
			revercecopy.map[63-i].color = revercecopy.map[63-i].color ? White : Black;
		}
		
		return revercecopy;
	}

	bool Chessboard::makeTurn(Turn turn , bool SafeMode) 
	{

		if(SafeMode == true)
		{
			std::vector<Turn> turns;
			getCurrentTurns(turns);
			for(Turn t : turns){
				if(turn == t){
					return makeTurn(turn,false);
				}			
			}
			return false;
			
		} else 
		{
            if(turn.type == TurnType::Normal){

                if(turn.start == Position(7,0))
                    LeftRooking = false;
                if(turn.start == Position(7,7)) // збивання рокіровок при ходах
                    RightRooking = false;
                if(turn.start == Position(7,4)){
                    LeftRooking = false;
                    RightRooking = false;
                }

                at(turn.end) = at(turn.start);
                at(turn.start).type = Emply;
                return true;
            }
            else if(turn.type == TurnType::Rooking)
            {
                if(turn.leftright == true)
                {

                    at(7,2) = at(7,4);
                    at(7,4).type = Emply;

                    at(7,3) = at(7,0);
                    at(7,0).type = Emply;
                    return true;

                }else
                {
                    at(7,6) = at(7,4);
                    at(7,4).type = Emply;

                    at(7,5) = at(7,7);
                    at(7,7).type = Emply;
                    return true;

                }

            }
            else if(turn.type == TurnType::FigureSwap)
            {
                at(turn.end) = at(turn.start);
                at(turn.start).type = Emply;
                at(turn.end).type = turn.figure;
                return true;
            }
		}
    }

	bool Chessboard::getCurrentTurns(std::vector<Turn> &turns)
	{

		turns.clear();

         for(unsigned x=0;x<8;x++)
		 {
             for(unsigned y = 0;y<8;y++){
				 assert(at(x,y).type<=6);
				 assert(at(x,y).color<=1);
				 
				 Position SP(x,y); // стартова позиція
				 
				 
				 if(at(x,y).type == FigureType::Emply || (at(x,y).type != FigureType::Emply && at(x,y).color == Black) )//пуста клітинка
					continue;
					
				 if(at(x,y).type == FigureType::Pawn)//пішак
                     {  // звичайні ходи
                       if(x > 1){
						if(at(x-1,y).type == FigureType::Emply ) // хід
							turns.push_back(Turn(SP , Position(x-1,y)));
						if(at(x-1,y).type == FigureType::Emply && at(x-2,y).type == FigureType::Emply && x == 6) // двойний хід
							turns.push_back(Turn(SP , Position(x-2,y)));
						if(at(x-1,y-1).type != FigureType::Emply && at(x-1,y-1).color != Color::White && y != 0) // ліва атака
							turns.push_back(Turn(SP , Position(x-1,y-1)));
						if(at(x-1,y+1).type != FigureType::Emply && at(x-1,y+1).color != Color::White && y != 7) // права атака
							turns.push_back(Turn(SP , Position(x-1,y+1)));
                       }
                       else if( x == 1)
                       { // якщо це хід з перетворенням
                           if(at(x-1,y).type == FigureType::Emply )
                           {
                               turns.push_back(Turn(SP , Position(x-1,y) , FigureType::Knight));
                               turns.push_back(Turn(SP , Position(x-1,y) , FigureType::Bishop));
                               turns.push_back(Turn(SP , Position(x-1,y) , FigureType::Rook));
                               turns.push_back(Turn(SP , Position(x-1,y) , FigureType::Queen));
                           }
                           if(at(x-1,y-1).type != FigureType::Emply && at(x-1,y-1).color != Color::White && y != 0)
                           {
                               turns.push_back(Turn(SP , Position(x-1,y-1) , FigureType::Knight));
                               turns.push_back(Turn(SP , Position(x-1,y-1) , FigureType::Bishop));
                               turns.push_back(Turn(SP , Position(x-1,y-1) , FigureType::Rook));
                               turns.push_back(Turn(SP , Position(x-1,y-1) , FigureType::Queen));
                           }
                            if(at(x-1,y+1).type != FigureType::Emply && at(x-1,y+1).color != Color::White && y != 7)
                           {
                             turns.push_back(Turn(SP , Position(x-1,y+1) , FigureType::Knight));
                             turns.push_back(Turn(SP , Position(x-1,y+1) , FigureType::Bishop));
                             turns.push_back(Turn(SP , Position(x-1,y+1) , FigureType::Rook));
                             turns.push_back(Turn(SP , Position(x-1,y+1) , FigureType::Queen));
                           }



                       }
					 } 
					 
					 else if(at(x,y).type == FigureType::Rook)  //тура
					 {
						 int offsetx[4] = {1 , 0 , -1 , 0};
						 int offsety[4] = {0 , 1 , 0 , -1};
						 unsigned cx;
						 unsigned cy;
						 for(int i=0;i<4;i++){
							cx = x;cy = y;
							while(1){
								cx += offsetx[i]; cy+= offsety[i]; // переміщаємося
								if( (at(cx,cy).type != Emply && at(cx,cy).color == White) || cx >= 8  || cy >= 8 ) // перевіряємо
									break;
								if( at(cx,cy).type != Emply && at(cx,cy).color == Black ){ // перевіряємо
									turns.push_back(Turn(SP,Position(cx,cy)));
									break;
								}
								turns.push_back(Turn(SP,Position(cx,cy))); // додаємо
							}
						 }
					 }

					 else if(at(x,y).type == FigureType::Bishop) //слон
					 {
						 int offsetx[4] = {1 , -1 , -1 , 1};
						 int offsety[4] = {1 , -1 , 1 , -1};
						 unsigned cx;
						 unsigned cy;
						 for(int i=0;i<4;i++){
							cx = x;cy = y;
							while(1){
								cx += offsetx[i]; cy+= offsety[i]; // переміщаємося
								if( (at(cx,cy).type != Emply && at(cx,cy).color == White) || cx >= 8  || cy >= 8 ) // перевіряємо
									break;
								else if( at(cx,cy).type != Emply && at(cx,cy).color == Black ){ // перевіряємо
									turns.push_back(Turn(SP,Position(cx,cy)));
									break;
								}
								turns.push_back(Turn(SP,Position(cx,cy))); // додаємо
							}
						 }
					 }

					 else if(at(x,y).type == FigureType::Knight) //конь
					 {
						 int offsetx[8] = {2 , 1 , -2 , 1 , 2, -1 , -2 ,-1};
						 int offsety[8] = {1 , 2 , 1 , -2, -1, 2 , -1 , -2};
						 unsigned cx;
						 unsigned cy;
						 for(int i=0;i<8;i++){
							cx = x;cy = y;
								cx += offsetx[i]; cy+= offsety[i]; // переміщаємося
								if( (at(cx,cy).type != Emply && at(cx,cy).color == White) || cx >= 8  || cy >= 8 ) // перевіряємо
									continue;
								turns.push_back(Turn(SP,Position(cx,cy))); // додаємо
						 }
					 }
					 
					 else if(at(x,y).type == FigureType::Queen) //каролева
					 {
						 const int offsetx[8] = {1 , -1 , -1 , 1 , 0 , 0 , 1 ,-1};
						 const int offsety[8] = {1 , -1 , 1 , -1 , 1 , -1, 0 , 0};
						 unsigned cx;
						 unsigned cy;
						 for(int i=0;i<8;i++){
							cx = x;cy = y;
							while(1){
								cx += offsetx[i]; cy+= offsety[i]; // переміщаємося
								if( (at(cx,cy).type != Emply && at(cx,cy).color == White) || cx >= 8  || cy >= 8 ) // перевіряємо
									break;
								else if( at(cx,cy).type != Emply && at(cx,cy).color == Black ){ // перевіряємо
									turns.push_back(Turn(SP,Position(cx,cy)));
									break;
								}
								turns.push_back(Turn(SP,Position(cx,cy))); // додаємо
							}
						 }
						
					 }

					 else if(at(x,y).type == FigureType::King) //король
					 {
						 const int offsetx[8] = {1 , -1 , -1 , 1 , 0 , 0 , 1 ,-1};
						 const int offsety[8] = {1 , -1 , 1 , -1 , 1 , -1, 0 , 0};
						 unsigned cx;
						 unsigned cy;
						 for(int i=0;i<8;i++){
							cx = x;cy = y;
                            cx += offsetx[i]; cy+= offsety[i]; // переміщаємося
								if( (at(cx,cy).type != Emply && at(cx,cy).color == White) || cx >= 8  || cy >= 8 ) // перевіряємо
									continue;
								turns.push_back(Turn(SP,Position(cx,cy))); // додаємо
						 }
					 }
			 }
		 }

         if(LeftRooking == true && at(7,1).type == Emply && at(7,2).type == Emply && at(7,3).type == Emply) // ліва рокіровка
             turns.push_back(Turn(true));
         if(RightRooking == true && at(7,5).type == Emply && at(7,6).type == Emply) // права рокіровка
             turns.push_back(Turn(false));

		 if(turns.empty())
			 return false;
		 return true;
		 
	}

	Chessboard& Chessboard::operator= (const Chessboard &from)
	{
		for(int i=0;i<64;i++){
			this->map[i] = from.map[i];
		}
		this->whiteTurn = from.whiteTurn; 
        this->LeftRooking = from.LeftRooking;
        this->RightRooking = from.RightRooking;
		return *this;	
	}

}
