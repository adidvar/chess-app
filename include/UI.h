#include <thread>
#include <vector>
#include "AbstractBot"
#include <mutex>

class InputUI : public Chess::AbstractBot
{
	sf::RenderWindow window;
	std::mutex window_mtx;
	std::vector<Chess::Turn> turnBuffer;
	std::mutex turnBuffer_mtx;
	
	public:
		InputUI();
		bool GetTurn(Chess::Turn &t,Chess::Chessboard &board);
	private:
		void RenderThread();
};