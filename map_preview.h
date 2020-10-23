#pragma once
#include <vector>
#include <mutex>
#include <array>
#include <SFML/Graphics.hpp>
#include "turngenerator.h"
#include "turn.h"

#include "board.h"
#include "player.h"

class Map_preview : public Player
{
public:
    Map_preview(Board board , Color main_color);
    virtual void UploadMap(Board board) final;
    virtual Turn GetTurn() final { return Turn();}
    void Exec();
private:
    std::mutex board_mutex;
};
