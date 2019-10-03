#include <vector>
#include <thread>
#include "AbstractBot.h"
#include <SFML/Graphics.hpp>
#include <mutex>
#include <array>

namespace Chess{

	class InputUI : public Chess::AbstractBot
	{
		public:
			InputUI();
			bool GetTurn(Chess::Turn &t,Chess::Chessboard &board);
            void RenderThread();
		private:

            Chess::Chessboard map;
            std::mutex map_mtx;
			std::vector<Chess::Turn> turnBuffer;
			std::mutex turnBuffer_mtx;
	};

};
