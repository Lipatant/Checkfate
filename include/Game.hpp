/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.hpp
** File description:
** -
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameData.hpp"
#include "Pieces/Player.hpp"

#define CHECKFATE_GAME_FPS 60

namespace checkfate {

class Game {
private: // sprites
    sf::Color _white = checkfate::white;
    sf::Color _black = checkfate::black;
    sf::RectangleShape _chessboard;
private:
    sf::Int64 _delta = 0;
    sf::Int64 _deltaPerUpdate = 1;
    sf::Clock _deltaClock;
private:
    bool _updateWindow(void);
    bool _updateGame(void);
    bool _updateDisplay(void);
    bool _loadTextures(void);
public:
    checkfate::Player player;
public:
    const sf::Vector2u screenSize = {256, 224};
    sf::RenderWindow window;
public:
    Game(void);
    ~Game() = default;
    //
    bool update(void);
    bool newGame(void);
};

};
