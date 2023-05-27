/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game/Update.cpp
** File description:
** -
*/

#include "Game.hpp"

#include <iostream>

static sf::Vector2f getWindowRatio(sf::Window const &window)
{
    sf::Vector2u size(window.getSize());
    sf::Vector2f ratio(1, 1);

    if (size.x > size.y)
        ratio.x = (float)size.y / size.x;
    else
        ratio.y = (float)size.x / size.y;
    return ratio;
}

namespace checkfate {

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
    sf::Vector2f tileSize = _chessboard.getSize();
    sf::Vector2u tileOnScreen(screenSize.x / tileSize.x + 1, \
        screenSize.x / tileSize.y + 1);
    sf::Vector2f windowRatio(getWindowRatio(window));
    sf::View view(window.getView());
    sf::FloatRect viewport(1, 1, windowRatio.x, windowRatio.y);

    viewport.left = (1 - viewport.width) / 2;
    viewport.top = (1 - viewport.height) / 2;
    view.setCenter(player.getDisplayedPosition());
    view.setViewport(viewport);
    window.setView(view);
    window.clear(_black);
    for (size_t x = 0; x < tileOnScreen.x; x++) {
        for (size_t y = 0; y < tileOnScreen.y; y++) {
            if ((x + y) % 2)
                continue;
            _chessboard.setPosition(x * tileSize.x, y * tileSize.y);
            window.draw(_chessboard);
        }
    }
    player.display(*this);
    window.display();
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
