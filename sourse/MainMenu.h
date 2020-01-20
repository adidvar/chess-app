#ifndef MainMenu_H
#define MainMenu_H

#include <string>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <chrono>
#include <thread>
#include <random>
#include "ChessController.h"
#include <array>
#include "UI.h"
#include "MyBot.h"
#include "NetPlayer.h"

using namespace sf;

class MainMenu
{
    sf::RenderWindow w;

    void My();
    void About();
    void CustomGame();
    void Play();
    void Net();
public:
    MainMenu();
    ~MainMenu();
};

#endif // MainMenu_H
