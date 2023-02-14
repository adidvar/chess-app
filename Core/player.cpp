#include "player.h"

Player::Player(Color main_color):
    main_color(main_color)
{

}

void Player::UploadMap(Board board, Turn lastturn)
{
    this->board =  board;
    this->lastturn = lastturn;
}
