//#include <vector>
//#include <thread>
#include "AbstractBot.h"
#include <SFML/Graphics.hpp>
#include <mutex>

namespace Chess{

	class InputUI : public Chess::AbstractBot
	{
		public:
			InputUI();
			bool GetTurn(Chess::Turn &t,Chess::Chessboard &board);
		private:
			void RenderThread();
			
            sf::RenderWindow window;
			std::mutex window_mtx;
			std::vector<Chess::Turn> turnBuffer;
			std::mutex turnBuffer_mtx;
	};

};
