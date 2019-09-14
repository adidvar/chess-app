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
		if(t1.start == t2.start && t1.end == t2.end)
			return true;
		return false;
	}
	
}
