#include <vector>
#include <assert.h>

namespace Chess{
	
	enum FigureType
	{
		   Emply = 0,
		   Pawn = 1,	
		   Knight = 2,
		   Bishop = 3,	
		   Rook = 4,
		   Queen = 5,
		   King = 6
    };

    enum MatchStatus
    {
        Now,
        Win,
        Lose
    };
	
	
	enum Color
	{
			White = 0,
			Black = 1
	};
		
	struct Figure
	{
		Figure(FigureType t , Color c):
		type(t),
		color(c)
		{}
        Figure(){}
		FigureType type;
		Color color;
	};
	
	struct Position
	{
		Position():
		x(0),
        y(0){}
		Position(unsigned xx , unsigned yy):
		x(xx),
        y(yy){}
		unsigned int x;
		unsigned int y;
	};

    enum TurnType
    {
        Normal,
        Rooking,
        FigureSwap
    };
	
	struct Turn
	{
        Turn(Position s , Position e): // normal
            start(s),end(e),type(Normal)
        {}
        Turn( bool lr ):
            type(Rooking),leftright(lr) // Rooking
        {}
        Turn( Position s , Position e , Figure fgr ): // Swap
            type(FigureSwap),start(s),end(e),figure(fgr)
        {}
        Turn(): // Zero
            type(Normal),start(),end()
        {}

        TurnType type;

        bool leftright;

		Position start;
		Position end;

        Figure figure;

	};
	
	bool operator==(const Position &t1 , const Position &t2);
	bool operator==(const Turn &t1 , const Turn &t2);
	
}

