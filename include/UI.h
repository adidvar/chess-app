#pragma once
#include "AbstractBot.h"
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <array>

namespace Chess{

	class InputUI : public Chess::AbstractBot
	{
		public:
			InputUI();
			bool GetTurn(Chess::Turn &t,Chess::Chessboard &board);
            void YouLose();
            void YouWin();
            void RenderThread();
		private:

            bool FigureChoiseMenu = false;
            //bool ChessMate = false;
            //std::mutex ChessMate_mutex;
            Chess::MatchStatus stat = MatchStatus::Now;
            std::mutex stat_mtx;
            MatchStatus GetStat()
            {
                MatchStatus st;
                stat_mtx.lock();
                st = stat;
                stat_mtx.unlock();
                return st;
            }

            Chess::Chessboard map;
            std::mutex map_mtx;
			std::vector<Chess::Turn> turnBuffer;
			std::mutex turnBuffer_mtx;
	};

};
