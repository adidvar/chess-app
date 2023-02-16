#include "player.h"

Player::Player(Color main_color):
    main_color(main_color)
{

}

void Player::UploadMap(Board board, Position lastturn)
{
    this->board =  board;
    this->lastturn = lastturn;
}
