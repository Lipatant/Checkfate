/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/APiece.cpp
** File description:
** -
*/

#include "Game.hpp"
#include "GameData.hpp"
#include "Pieces/APiece.hpp"
#include "IsValid.hpp"

#include <iostream>

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

namespace checkfate {

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
    _sprite.setSize(CHECKFATE_TILE);
    _sprite.setFillColor(sf::Color::Red);
}

void APiece::moveForce(int const x, int const y)
{
    return moveForce(checkfate::Position(x, y));
}

void APiece::moveForce(checkfate::Position const position)
{
    _positionPrevious = _position;
    _position = position;
    _isMoving = true;
    _clock.restart();
}

std::list<checkfate::Move> APiece::listMoves(bool const onlyLegal)
{
    std::list<checkfate::Move> moves;

    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++)
            if ((x != 0 || y != 0) && isValidMouseTile(_position.x + x, \
                _position.y + y))
                moves.push_back(checkfate::Move(checkfate::Position( \
                    _position.x + x, _position.y + y)));
    return moves;
}

std::list<checkfate::Move> APiece::listMoves(void)
{
    return listMoves(true);
}

checkfate::Move APiece::bestMove(void)
{
    return checkfate::Move(_position);
}

checkfate::Position APiece::getPosition(void)
{
    return _position;
}

checkfate::PositionPrecise APiece::getDisplayedPosition(void)
{
    sf::Vector2f spritePosition(0, 0);

    if (_displayed && _isMoving) {
        sf::Time clockTime = _clock.getElapsedTime();
        if (clockTime.asMilliseconds() >= 100)
            _isMoving = false;
        else {
            _animatedPosition.x = _positionPrevious.x + \
                static_cast<float>(_position.x - _positionPrevious.x) * \
                (clockTime.asMilliseconds() / 100.0);
            _animatedPosition.y = _positionPrevious.y + \
                static_cast<float>(_position.y - _positionPrevious.y) * \
                (clockTime.asMilliseconds() / 100.0);
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

bool APiece::display(checkfate::Game &game)
{
    _sprite.setPosition(getDisplayedPosition());
    game.window.draw(_sprite);
    _displayed = true;
    return true;
}

};
