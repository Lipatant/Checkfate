/*
** Emplynk, Virgile (Lipatant) Berrier, 2023
** Emplynk/Game.hpp
** File description:
** -
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define CHECKFATE_GAME_FPS 60

namespace checkfate {

class Game {
private:
    sf::Int64 _delta = 0;
    sf::Int64 _deltaPerUpdate = 1;
    sf::Clock _deltaClock;
private:
    bool _updateWindow(void);
    bool _updateGame(void);
    bool _updateDisplay(void);
public:
    const sf::Vector2u screenSize = {256, 224};
    sf::Window window;
public:
    Game(void);
    ~Game() = default;
    //
    bool update(void);
};

};
