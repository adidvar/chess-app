#pragma once
#include <APlayer.h>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <array>

namespace Chess{

class InputUI : public Chess::APlayer
{
public:
    InputUI(Color c);
    bool GetTurn(Chess::Turn &t);
    void MapEvent(Chessboard board);
    void YouLose();
    void YouWin();

private:
    void RenderThread();

    MatchStatus GetStat()
    {
        MatchStatus st;
        stat_mtx.lock();
        st = stat;
        stat_mtx.unlock();
        return st;
    }

    Chess::MatchStatus stat = MatchStatus::Now;
    std::mutex stat_mtx;
    Chess::Chessboard map;
    std::mutex map_mtx;
    std::vector<Chess::Turn> turnBuffer;
    std::mutex turnBuffer_mtx;
};

};
