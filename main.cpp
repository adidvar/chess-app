#include <iostream>
#include <MyBot.h>
#include <SFML/Graphics.hpp>

using namespace std;

class Input : public Chess::AbstractBot
{
	sf::RenderWindow &w;
	public:
	Input(sf::RenderWindow& r):
	w(r){;};
	bool GetTurn(Chess::Turn &t,Chess::Chessboard &board);	
};

bool Input::GetTurn(Chess::Turn &t,Chess::Chessboard &board)
{
	std::vector<Chess::Position> turnpair;
	
	sf::Event e;
	
	while(turnpair.size() < 2){
		while(w.pollEvent(e))
			{
				if (e.type == sf::Event::MouseButtonPressed)
					turnpair.push_back(Chess::Position(e.mouseButton.y / 64,e.mouseButton.x / 64));
			}
	}
	t = Chess::Turn(turnpair[0],turnpair[1]);
	turnpair.clear();
	return true;
}

int main()
{

	sf::Texture black_textures[7];
	sf::Texture white_textures[7];
	black_textures[1].loadFromFile("Img/Black_Pawn.png");
	black_textures[2].loadFromFile("Img/Black_Knight.png");
	black_textures[3].loadFromFile("Img/Black_Bishop.png");
	black_textures[4].loadFromFile("Img/Black_Rook.png");
	black_textures[5].loadFromFile("Img/Black_Queen.png");
	black_textures[6].loadFromFile("Img/Black_King.png");
	white_textures[1].loadFromFile("Img/White_Pawn.png");
	white_textures[2].loadFromFile("Img/White_Knight.png");
	white_textures[3].loadFromFile("Img/White_Bishop.png");
	white_textures[4].loadFromFile("Img/White_Rook.png");
	white_textures[5].loadFromFile("Img/White_Queen.png");
	white_textures[6].loadFromFile("Img/White_King.png");
	
	sf::RenderWindow w(sf::VideoMode(512,512,32),"Chess");
	
	Chess::MyBot b1;
	//Chess::MyBot b2;
	Input b2(w);
	Chess::ChessController c(b1,b2);
	Chess::Chessboard board;
	
	sf::RectangleShape rect(sf::Vector2f(64,64) );
	sf::Sprite sprite;
	
	
    while(1){
		
		w.clear();

		
		board = c.GetGlobalBoard();
		
		//відмальвка кольору
		bool IsWhite = false;
		for (int x = 0; x < 8; x++) 
		{
			for (int y = 0; y < 8; y++) 
			{
				IsWhite = !IsWhite;
				rect.setFillColor( IsWhite ? sf::Color(200,200,200) : sf::Color(50,50,50));
				rect.setPosition(sf::Vector2f(x*64,y*64));
				w.draw(rect);
			}
			IsWhite = !IsWhite;
		}
		
		for (int x = 0; x < 8; x++) 
		{
			for (int y = 0; y < 8; y++) 
			{
				sprite.setPosition(sf::Vector2f(x*64 + 7,y*64 + 7));
				sf::Texture &t = board.at(y,x).color == Chess::Color::White ? white_textures[board.at(y,x).type] : black_textures[board.at(y,x).type];
				sprite.setTexture(t , false);
				
				if(board.at(y,x).type != Chess::Emply)
					w.draw(sprite);
				
			}
		
		}
		
		w.display();
		
		c.NextMove();

		
    }
    return 0;
}