/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.cpp
** File description:
** -
*/

#include "Game.hpp"
#include "GameData.hpp"

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

namespace checkfate {

Game::Game(void) : \
    window(sf::VideoMode(SPLIT_V(screenSize)), "Checkfate")
{
    _deltaClock.restart();
    if (!window.isOpen())
        return;
    if (!_loadTextures()) {
        window.close();
        return;
    }
    _deltaPerUpdate = 1e6 / CHECKFATE_GAME_FPS;
    window.setFramerateLimit(CHECKFATE_GAME_FPS);
}

bool Game::_loadTextures(void)
{
    _chessboard.setFillColor(_white);
    _chessboard.setSize(CHECKFATE_TILE);
    return true;
}

bool Game::newGame(void)
{
    player.place(0, 0);
    return true;
}

};
