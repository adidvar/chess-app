#include "Chess.h"

namespace Chess{
	
	bool operator==(const Position &t1 , const Position &t2)
		{
			if(t1.x == t2.x && t1.y == t2.y)
				return true;
			return false;
		}
	
	bool operator==(const Turn &t1 , const Turn &t2)
	{
        if(t1.type == t2.type)
        {
            if(t1.type == TurnType::Normal)
            {
                if(t1.start == t2.start && t1.end == t2.end)
                    return true;
                else
                    return false;
            } else if(t1.type == TurnType::Rooking)
            {
                if(t1.leftright == t2.leftright)
                    return true;
                else
                    return false;
            } else if(t1.type == TurnType::FigureSwap)
            {
                if(t1.start == t2.start && t1.end == t2.end && t1.figure == t2.figure)
                    return true;
                else
                    return false;
            }
        }
		return false;
	}
}
