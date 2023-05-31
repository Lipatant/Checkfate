/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/APiece.cpp
** File description:
** -
*/

#include <algorithm>
#include <cstdlib>
#include "Flags.hpp"
#include "Game.hpp"
#include "GameData.hpp"
#include "Pieces/APiece.hpp"
#include "IsValid.hpp"

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

#define PIECE_MOVEMENT_MODIFIER_ROOK { \
    sf::Vector2i(1,0), sf::Vector2i(-1,0), \
    sf::Vector2i(0,1), sf::Vector2i(0,-1)}

#define PIECE_MOVEMENT_MODIFIER_BISHOP { \
    sf::Vector2i(1,1), sf::Vector2i(-1,1), \
    sf::Vector2i(1,-1), sf::Vector2i(-1,-1)}

namespace checkfate {

void APiece::assignGame(Game *game)
{
    _game = game;
}

void APiece::place(int const x, int const y)
{
    return place(checkfate::Position(x, y));
}

void APiece::place(checkfate::Position const position)
{
    _position = position;
    _positionPrevious = position;
    _animatedPosition.x = position.x;
    _animatedPosition.y = position.y;
    _clock.restart();
    _isMoving = false;
    _nextMove.x = -1;
    _nextMove.y = -1;
    _nextMoveDelay = 1;
    if (_isPlayer)
        _isWhite = (position.x + position.y) % 2;
    else
        _isWhite = std::rand() % 2;
    _isWhitePrevious = _isWhite;
}

size_t APiece::getTier(bool const forDisplay) const
{
    if (forDisplay && !_isPlayer && _game) {
        bool playerIsWhite = _game->player.isWhite();
        if (_isWhite != playerIsWhite && \
            _game->upgrades.has("challenge_hidden_opposite"))
            return 0;
    }
    return (forDisplay && _hideTier) ? 0 : _tier;
}

size_t APiece::getTier(void) const
{
    return getTier(false);
}

void APiece::moveForce(int const x, int const y)
{
    return moveForce(checkfate::Position(x, y));
}

void APiece::moveForce(checkfate::Position const position)
{
    checkfate::Position gap(
        abs(_position.x - position.x),
        abs(_position.y - position.y)
    );

    if (gap.x > gap.y)
        _animationSpeed = 100 * gap.x;
    else
        _animationSpeed = 100 * gap.y;
    _positionPrevious = _position;
    _position = position;
    _isMoving = true;
    _clock.restart();
    _isWhitePrevious = _isWhite;
    if (_isPlayer)
        _isWhite = (position.x + position.y) % 2;
}

bool APiece::_addMovement(std::list<checkfate::Move> &moves, checkfate::Move \
    move, bool const onlyLegal)
{
    bool playerGoThrough = true;

    if (!isValidMouseTile(move.position)) return false;
    if (_game) {
        move.distance = abs(_game->player.getPosition().x - move.position.x) \
            + abs(_game->player.getPosition().y - move.position.y);
        if (!_isPlayer) {
            for (auto &enemy : _game->enemies) {
                if (enemy->getPosition() == move.position) {
                    if (onlyLegal) return false; else move.isLegal = false;
                }
            }
            for (auto &enemy : _game->enemiesIncomming) {
                if (enemy->getPosition() == move.position) {
                    if (onlyLegal) return false; else move.isLegal = false;
                }
            }
        } else {
            for (auto &enemy : _game->enemies)
                if (enemy->getPosition() == move.position)
                    playerGoThrough = false;
        }
    }
    moves.push_back(move);
    return playerGoThrough;
}

std::list<checkfate::Move> APiece::listMoves(bool const onlyLegal)
{
    std::list<checkfate::Move> moves;
    size_t rookLevel = _rookLevel;
    size_t bishopLevel = _bishopLevel;
    size_t knightLevel = _knightLevel;

    _hideTier = false;
    if (_isPlayer && _game) {
        if (_game->upgrades.has("player_not_moving") && _game->combo > 0)
            _addMovement(moves, checkfate::Move(checkfate::Position( \
                _position.x, _position.y)), onlyLegal);
        std::string coloredPrefix = _isWhite ? "W" : "B";
        if (_game->upgrades.has(coloredPrefix + "_player_knight"))
            knightLevel += 1;
        if (_game->combo > 1 && \
            _game->upgrades.has(coloredPrefix + "_player_combo_bishop"))
            bishopLevel += std::min<size_t>(_game->combo - 1, 2);
        if (_game->combo > 1 && \
            _game->upgrades.has(coloredPrefix + "_player_combo_rook"))
            rookLevel += std::min<size_t>(_game->combo - 1, 2);
    }
    /// TOWER LEVEL
    for (auto const move: PIECE_MOVEMENT_MODIFIER_ROOK)
        for (size_t i = 1; i <= rookLevel; i++)
            if (!_addMovement(moves, checkfate::Move(checkfate::Position( \
                _position.x + i * move.x, \
                _position.y + i * move.y)), onlyLegal))
                break;
    /// BISHOP LEVEL
    for (auto const move: PIECE_MOVEMENT_MODIFIER_BISHOP)
        for (size_t i = 1; i <= bishopLevel; i++)
            if (!_addMovement(moves, checkfate::Move(checkfate::Position( \
                _position.x + i * move.x, \
                _position.y + i * move.y)), onlyLegal))
                break;
    /// KNIGHT LEVEL
    if (knightLevel > 0) {
        for (int const x : {-1, 1}) {
            for (int const y : {-1, 1}) {
                _addMovement(moves, checkfate::Move(checkfate::Position( \
                    _position.x + 2 * x, _position.y + 1 * y)), onlyLegal);
                _addMovement(moves, checkfate::Move(checkfate::Position( \
                    _position.x + 1 * x, _position.y + 2 * y)), onlyLegal);
            }
        }
    }
    return moves;
}

std::list<checkfate::Move> APiece::listMoves(void)
{
    return listMoves(true);
}

checkfate::Move APiece::bestMove(void)
{
    std::list<checkfate::Move> moves = listMoves();

    if (moves.empty())
        return checkfate::Move(_position);
    moves.sort();
    return moves.front();
}

std::string const &APiece::getName(void) const
{
    return _name;
}

checkfate::Position APiece::getPosition(void) const
{
    return _position;
}

checkfate::PositionPrecise APiece::getDisplayedPosition(void)
{
    sf::Vector2f spritePosition(0, 0);

    if (_displayed && _isMoving) {
        sf::Time clockTime = _clock.getElapsedTime();
        if (clockTime.asMilliseconds() >= _animationSpeed)
            _isMoving = false;
        else {
            _animatedPosition.x = _positionPrevious.x + \
                static_cast<float>(_position.x - _positionPrevious.x) * \
                (clockTime.asMilliseconds() / \
                static_cast<float>(_animationSpeed));
            _animatedPosition.y = _positionPrevious.y + \
                static_cast<float>(_position.y - _positionPrevious.y) * \
                (clockTime.asMilliseconds() / \
                static_cast<float>(_animationSpeed));
        }
    }
    if (_displayed)
        _displayed = false;
    if (!_isMoving) {
        spritePosition.x = _position.x * CHECKFATE_TILE_X;
        spritePosition.y = _position.y * CHECKFATE_TILE_Y;
    } else {
        spritePosition.x = _animatedPosition.x * CHECKFATE_TILE_X;
        spritePosition.y = _animatedPosition.y * CHECKFATE_TILE_Y;
    }
    return spritePosition;
}

checkfate::Position &APiece::nextMove(void)
{
    return _nextMove;
}

size_t &APiece::nextMoveDelay(void)
{
    return _nextMoveDelay;
}

bool &APiece::isWhite(void)
{
    return _isWhite;
}

bool APiece::isWhite(void) const
{
    return _isWhite;
}

sf::Color APiece::_colorTransition(sf::Color const pri, sf::Color const sec)
{
    sf::Time clockTime = _clock.getElapsedTime();
    int priR = pri.r; int priG = pri.g; int priB = pri.b;
    int secR = sec.r; int secG = sec.g; int secB = sec.b;
    int newR, newG, newB;

    newR = secR + static_cast<float>(priR - secR) * \
        (clockTime.asMilliseconds() / static_cast<float>(_animationSpeed));
    newG = secG + static_cast<float>(priG - secG) * \
        (clockTime.asMilliseconds() / static_cast<float>(_animationSpeed));
    newB = secB + static_cast<float>(priB - secB) * \
        (clockTime.asMilliseconds() / static_cast<float>(_animationSpeed));
    return sf::Color(newR, newG, newB);
}

bool APiece::display(void)
{
    if (!_game) return false;
    sf::Vector2f spritePosition = getDisplayedPosition();
    sf::Color priColor = _isWhite ? checkfate::white : checkfate::black;
    sf::Color secColor = _isWhite ? checkfate::black : checkfate::white;
    size_t tier;

    if (_isPlayer && !_isMoving && _game->gameState == GameState::Lost)
        return false;
    if (_isMoving && _isWhite != _isWhitePrevious) {
        sf::Time clockTime = _clock.getElapsedTime();
        if (clockTime.asMilliseconds() < _animationSpeed) {
            sf::Color temp(_colorTransition(priColor, secColor));
            secColor = _colorTransition(secColor, priColor);
            priColor = temp;
        }
    }
    spritePosition.y -= CHECKFATE_TILE_Y * 1.5 - 2;
    _game->pieceTextureRect.left = _spriteIndex * \
        _game->pieceTextureRect.width;
    _game->pieceSprite.setTextureRect(_game->pieceTextureRect);
    _game->pieceSprite.setColor(secColor);
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            if (x == 0 && y == 0)
                continue;
            _game->pieceSprite.setPosition({spritePosition.x + x, \
                spritePosition.y + y});
            _game->window.draw(_game->pieceSprite);
        }
    }
    _game->pieceSprite.setPosition(spritePosition);
    _game->pieceSprite.setColor(priColor);
    _game->window.draw(_game->pieceSprite);
    tier = getTier(true);
    if (!_isPlayer && tier != 0) {
        _game->pieceTierTextureRect.left = tier * \
            _game->pieceTierTextureRect.width;
        _game->pieceTierSprite.setTextureRect(_game->pieceTierTextureRect);
        _game->pieceTierSprite.setColor(secColor);
        for (int x = -1; x <= 1; x++) {
            for (int y = -1; y <= 1; y++) {
                if (x == 0 && y == 0)
                    continue;
                _game->pieceTierSprite.setPosition({spritePosition.x + x, \
                    spritePosition.y + y});
                _game->window.draw(_game->pieceTierSprite);
            }
        }
        _game->pieceTierSprite.setPosition(spritePosition);
        _game->pieceTierSprite.setColor(priColor);
        _game->window.draw(_game->pieceTierSprite);
    }
    _displayed = true;
    return true;
}

};
