/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.cpp
** File description:
** -
*/

#include "Error.hpp"
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
    if (!pieceTexture.loadFromFile("graphics/Pieces.png"))
        return error::raiseBool("Couldn't load graphics/Pieces.png texture");
    pieceSprite.setTexture(pieceTexture);
    pieceTextureRect = pieceSprite.getTextureRect();
    pieceTextureRect.width = CHECKFATE_TILE_X;
    pieceTextureRect.height = CHECKFATE_TILE_Y * 2;
    _chessboard.setFillColor(_white);
    _chessboard.setSize(CHECKFATE_TILE);
//    _chessboardTarget.setFillColor(sf::Color(_white.r, _black.g, _black.b));
    _chessboardTarget.setFillColor(sf::Color::Transparent);
    _chessboardTarget.setOutlineThickness(2);
    _chessboardTarget.setSize({CHECKFATE_TILE_X - 4, CHECKFATE_TILE_Y - 4});
    return true;
}

bool Game::newGame(void)
{
    player.place(0, 0);
    _updatePlayerMoves();
    return true;
}

};
