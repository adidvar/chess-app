#pragma once
#include "APlayer.h"
#include <vector>
#include <thread>
#include <mutex>
#include <array>
#include <SFML/Graphics.hpp>

namespace Chess{

class InputUI : public Chess::APlayer
{
public:
    InputUI(Color c);
    ~InputUI();
    bool GetTurn(Chess::Turn &t);
    void MapEvent(Chessboard board);
    void FinishEvent(Color c);

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
    std::mutex IsLive_mtx;
    bool IsLive = true;
    Chess::MatchStatus stat = MatchStatus::Now;
    std::mutex stat_mtx;
    Chess::Chessboard map;
    std::mutex map_mtx;
    std::vector<Chess::Turn> turnBuffer;
    std::mutex turnBuffer_mtx;
};

};
