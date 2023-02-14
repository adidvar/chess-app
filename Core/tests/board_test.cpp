#include "gtest/gtest.h"
#include "board.h"

class Board_Test : public Board
{
public:
	void Testing()
	{

		for(int i=0;i<64;i++)
		{
			Figures fig = 0;
			Color color = 0;

			for(Figures fig = 0; fig < 7 ; fig++){
				for(Color color = 0; color < 2; color++){

					Set(i,fig,color);

					ASSERT_TRUE(Test(fig,i)) << "Error figure read Test()";
					ASSERT_TRUE(TestColor(color,i)) << "Error color read TestColor()";
					ASSERT_TRUE(GetFigure(i) == fig) << "Error figure read GetFigure()";
					ASSERT_TRUE(GetColor(i) == color) << "Error color read GetColor()";
					
				}
			}
		}

    Set(0,pawn,white);
    Set(1,knight,black);
    Swap(0,1);
    ASSERT_TRUE(Test(pawn,1));
    ASSERT_TRUE(Test(knight,0));
    ASSERT_TRUE(TestColor(black,0));
    ASSERT_TRUE(TestColor(white,1));
	}
};

TEST(Core,Board)
{
	Board_Test test;
	test.Testing();
}
