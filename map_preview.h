#pragma once
#include <vector>

#include <mutex>
#include <condition_variable>
#include <atomic>

#include <array>
#include <SFML/Graphics.hpp>
#include "turngenerator.h"
#include "turnexecutor.h"
#include "turn.h"

#include "board.h"
#include "player.h"

#include <iostream>

class Map_preview : public Player
{

public:
    Map_preview(Board board , Color main_color);
    virtual void UploadMap(Board board, Turn lastturn) final;
    virtual Turn GetTurn() final;
    void Exec();
private:
    std::mutex board_mutex;
    std::atomic_bool handle_flag;
    std::condition_variable handle;
    std::mutex turn_mutex;
    Turn turn;

    void CaptureEvent(sf::RenderWindow &w);
    Figures PickRender(sf::RenderWindow &w);
};
