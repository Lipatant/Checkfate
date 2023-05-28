/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game/Update.cpp
** File description:
** -
*/

#include "Game.hpp"
#include "IsValid.hpp"

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

bool Game::_updatePlayerMoves(void)
{
    playerMoves = player.listMoves();
    return true;
}

bool Game::_updateGame(void)
{
    if (_mouseClickState == checkfate::InputStateComplex::JustPressed) {
        if (isValidMouseTile(_mouseTile)) {
            for (auto const &move: playerMoves) {
                if (move.position == _mouseTile) {
                    player.moveForce(_mouseTile);
                    _updatePlayerMoves();
                    break;
                }
            }
        }
    }
    return true;
}

bool Game::_updateMouse(void)
{
    sf::Vector2i positionPixels = sf::Mouse::getPosition(window);

    _mouseUI = window.mapPixelToCoords(positionPixels, _view);
    _mouseTile.x = _mouseUI.x / CHECKFATE_TILE_X;
    if (_mouseUI.x < 0)
        _mouseTile.x = -1;
    _mouseTile.y = _mouseUI.y / CHECKFATE_TILE_Y;
    if (_mouseUI.y < 0)
        _mouseTile.y = -1;
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
    checkfate::PositionPrecise playerDisplayed(player.getDisplayedPosition());

    _view = window.getView();
    _viewport = sf::FloatRect(1, 1, windowRatio.x, windowRatio.y);
    _viewport.left = (1 - _viewport.width) / 2;
    _viewport.top = (1 - _viewport.height) / 2;
//    if (playerDisplayed.x < screenSize.x / 2)
//        playerDisplayed.x = screenSize.x / 2;
//    else if (playerDisplayed.x >= CHECKFATE_CHESSBOARD_SIZE_X * CHECKFATE_TILE_X - screenSize.x / 2)
//        playerDisplayed.x = CHECKFATE_CHESSBOARD_SIZE_X * CHECKFATE_TILE_X - screenSize.x / 2;
//    if (playerDisplayed.y < screenSize.y / 2)
//        playerDisplayed.y = screenSize.y / 2;
//    else if (playerDisplayed.y >= CHECKFATE_CHESSBOARD_SIZE_Y * CHECKFATE_TILE_Y - screenSize.y / 2)
//        playerDisplayed.y = CHECKFATE_CHESSBOARD_SIZE_Y * CHECKFATE_TILE_Y - screenSize.y / 2;
    _view.setCenter(playerDisplayed);
    _view.setViewport(_viewport);
    _updateMouse();
    window.setView(_view);
    window.clear(sf::Color::Black);
    for (size_t x = 0; x < checkfate::chessboardSizeX; x++) {
        for (size_t y = 0; y < checkfate::chessboardSizeY; y++) {
            if ((x + y) % 2)
                _chessboard.setFillColor(_white);
            else
                _chessboard.setFillColor(_black);
            _chessboard.setPosition(x * tileSize.x, y * tileSize.y);
            window.draw(_chessboard);
        }
    }
    for (auto const move: playerMoves) {
        _chessboardTarget.setOutlineColor(sf::Color(_black.r, _black.g, \
            _white.b));
        _chessboardTarget.setPosition(move.position.x * tileSize.x + 2, \
            move.position.y * tileSize.y + 2);
        window.draw(_chessboardTarget);
    }
    if (isValidMouseTile(_mouseTile)) {
        _chessboardTarget.setOutlineColor(sf::Color(_white.r, _black.g, \
            _black.b));
        _chessboardTarget.setPosition(_mouseTile.x * tileSize.x + 2, \
            _mouseTile.y * tileSize.y + 2);
        window.draw(_chessboardTarget);
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
