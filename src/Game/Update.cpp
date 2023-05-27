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
        if (event.type == sf::Event::GainedFocus)
            hasFocus = true;
        if (event.type == sf::Event::LostFocus)
            hasFocus = false;
    }
    return true;
}

bool Game::_updateGame(void)
{
    if (_mouseClickState == checkfate::InputStateComplex::JustPressed) {
        if (_mouseTile.x >= 0 && _mouseTile.y >= 0)
            player.moveForce(_mouseTile);
    }
    return true;
}

bool Game::_updateMouse(void)
{
    sf::Vector2i positionPixels = sf::Mouse::getPosition(window);

    _mouseUI = window.mapPixelToCoords(positionPixels, _view);
    _mouseTile.x = _mouseUI.x / CHECKFATE_TILE_X;
    _mouseTile.y = _mouseUI.y / CHECKFATE_TILE_Y;
    if (hasFocus && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (_mouseClick)
            _mouseClickState = checkfate::InputStateComplex::AlreadyPressed;
        else
            _mouseClickState = checkfate::InputStateComplex::JustPressed;
        _mouseClick = true;
    } else {
        if (!_mouseClick)
            _mouseClickState = checkfate::InputStateComplex::AlreadyReleased;
        else
            _mouseClickState = checkfate::InputStateComplex::JustReleased;
        _mouseClick = false;
    }
    return true;
}

bool Game::_updateDisplay(void)
{
    sf::Vector2f tileSize = _chessboard.getSize();
    sf::Vector2f windowRatio(getWindowRatio(window));

    _view = window.getView();
    _viewport = sf::FloatRect(1, 1, windowRatio.x, windowRatio.y);
    _viewport.left = (1 - _viewport.width) / 2;
    _viewport.top = (1 - _viewport.height) / 2;
    _view.setCenter(player.getDisplayedPosition());
    _view.setViewport(_viewport);
    _updateMouse();
    window.setView(_view);
    window.clear(sf::Color::Black);
    for (size_t x = 0; x < checkfate::chessboardSizeX; x++) {
        for (size_t y = 0; y < checkfate::chessboardSizeY; y++) {
            if ((x + y) % 2)
                _chessboard.setFillColor(checkfate::white);
            else
                _chessboard.setFillColor(checkfate::black);
            if (x == _mouseTile.x && y == _mouseTile.y)
                _chessboard.setFillColor(sf::Color::Blue);
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
