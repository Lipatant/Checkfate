/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.cpp
** File description:
** -
*/

#include "Error.hpp"
#include "Game.hpp"

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

namespace checkfate {

Game::Game(void) : \
    window(sf::VideoMode(SPLIT_V(screenSize)), "Checkfate")
{
    _deltaClock.restart();
    if (!window.isOpen())
        return;
    _deltaPerUpdate = 1e6 / CHECKFATE_GAME_FPS;
    std::cout << _deltaPerUpdate << std::endl;
    window.setFramerateLimit(CHECKFATE_GAME_FPS);
}

bool Game::_updateWindow(void)
{
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    return true;
}

bool Game::_updateGame(void)
{
    return true;
}

bool Game::_updateDisplay(void)
{
    return true;
}

bool Game::update(void)
{
    _delta += _deltaClock.getElapsedTime().asMicroseconds();
    _updateWindow();
    _deltaClock.restart();
    while (_delta >= _deltaPerUpdate) {
        _updateGame();
        _delta -= _deltaPerUpdate;
    }
    _updateDisplay();
    return window.isOpen();
}

};
