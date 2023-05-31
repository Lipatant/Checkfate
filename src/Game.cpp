/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.cpp
** File description:
** -
*/

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Error.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Tower.hpp"

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

namespace checkfate {

Game::Game(void) : \
    window(sf::VideoMode::getDesktopMode(), "Checkfate", \
        sf::Style::Resize | sf::Style::Close)
{
    _deltaClock.restart();
    if (!window.isOpen())
        return;
    if (!_loadTextures()) {
        window.close();
        return;
    }
    std::srand(std::time(nullptr));
    _deltaPerUpdate = 1e6 / CHECKFATE_GAME_FPS;
    window.setFramerateLimit(CHECKFATE_GAME_FPS);
    gameState = GameState::Playing;
}

bool Game::_loadTextures(void)
{
    if (!pieceTexture.loadFromFile("graphics/Pieces.png"))
        return error::raiseBool("Couldn't load graphics/Pieces.png texture");
    if (!_chessboardTexture.loadFromFile("graphics/Tiles.png"))
        return error::raiseBool("Couldn't load graphics/Tiles.png texture");
    if (!pieceTierTexture.loadFromFile("graphics/Tiers.png"))
        return error::raiseBool("Couldn't load graphics/Tiers.png texture");
    if (!_font.loadFromFile("fonts/WayfarersToyBoxRegular.ttf"))
        return error::raiseBool("Couldn't load fonts/WayfarersToyBoxRegular.ttf font");
    _scoreText.setFont(_font);
    _scoreText.setOutlineThickness(8);
    // pieceSprite
    pieceSprite.setTexture(pieceTexture);
    pieceTextureRect = pieceSprite.getTextureRect();
    pieceTextureRect.width = CHECKFATE_TILE_X;
    pieceTextureRect.height = CHECKFATE_TILE_Y * 2;
    // pieceTierSprite
    pieceTierSprite.setTexture(pieceTierTexture);
    pieceTierTextureRect = pieceSprite.getTextureRect();
    pieceTierTextureRect.width = CHECKFATE_TILE_X;
    pieceTierTextureRect.height = CHECKFATE_TILE_Y * 2;
    // _chessboardSprite
    _chessboardSprite.setTexture(_chessboardTexture);
    _chessboardTextureRect = _chessboardSprite.getTextureRect();
    _chessboardTextureRect.width = CHECKFATE_TILE_X;
    _chessboardTextureRect.height = CHECKFATE_TILE_Y;
    _chessboard.setFillColor(_white);
    _chessboard.setSize(CHECKFATE_TILE);
    _chessboardTarget.setFillColor(sf::Color::Transparent);
    _chessboardTarget.setOutlineThickness(2);
    _chessboardTarget.setSize({CHECKFATE_TILE_X - 4, CHECKFATE_TILE_Y - 4});
    return true;
}

bool Game::newGame(void)
{
    _score = 0;
    combo = 0;
    _enemiesMax = 10;
    _enemiesTier = 2;
    _scoreForUpgrade = 0;
    player.place(CHECKFATE_CHESSBOARD_SIZE_X / 2, \
        CHECKFATE_CHESSBOARD_SIZE_Y / 2);
    gameState = GameState::Playing;
    player.assignGame(this);
    enemiesIncomming.push_back(checkfate::createPiece<checkfate::APiece>());
    enemiesIncomming.clear();
    enemies.push_back(checkfate::createPiece<checkfate::APiece>());
    enemies.clear();
    upgrades.clear();
    upgradesAvailable.clear();
    _updatePlayerMoves();
    return true;
}

void Game::addToScore(size_t const amount)
{
    _score += amount;
    _scoreForUpgrade += amount;
}

bool Game::_addEnnemy(void)
{
    if (enemies.size() + enemiesIncomming.size() >= \
        std::min<size_t>(_enemiesMax, CHECKFATE_CHESSBOARD_SIZE_X * \
        CHECKFATE_CHESSBOARD_SIZE_Y - 9)) return false;
    checkfate::Position placement;
    bool placementConflict;
    int randInt = std::rand() % 10;

    if (randInt < 4)
        enemiesIncomming.push_back(checkfate::createPiece< \
            checkfate::Bishop>(0));
    else if (randInt < 8)
        enemiesIncomming.push_back(checkfate::createPiece< \
            checkfate::Tower>(upgrades.has("challenge_stronger_rook")));
    else if (randInt < 9)
        enemiesIncomming.push_back(checkfate::createPiece< \
            checkfate::Queen>(0));
    else
        enemiesIncomming.push_back(checkfate::createPiece< \
            checkfate::Knight>(0));
    do {
        placement.x = std::rand() % CHECKFATE_CHESSBOARD_SIZE_X;
        placement.y = std::rand() % CHECKFATE_CHESSBOARD_SIZE_Y;
        placementConflict = false;
        if (placement == player.getPosition())
            placementConflict = true;
        for (auto const &enemy: enemies) {
            if (placement == enemy->getPosition()) {
                placementConflict = true;
                break;
            }
        }
        for (auto const &enemy: enemiesIncomming) {
            if (placement == enemy->getPosition()) {
                placementConflict = true;
                break;
            }
        }
    } while (placementConflict);
    enemiesIncomming.back()->setTier(enemiesIncomming.back()->getTier() + (std::rand() % _enemiesTier) + 1);
    enemiesIncomming.back()->place(placement);
    enemiesIncomming.back()->nextMoveDelay() = 2;
    enemiesIncomming.back()->assignGame(this);
    return true;
}

};
