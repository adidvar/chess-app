#pragma once
#include <mutex>
#include <thread>
#include <AbstractObs.h>
#include <SFML/Graphics.hpp>



namespace Chess {

class ObserverIU : public AbstractObserver
{
public:
    ObserverIU();
    void MapEvent(Chess::Chessboard map);
    void RenderThread();
private:
    Chess::Chessboard map;
    std::mutex map_mtx;
};

}
