/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/APiece.cpp
** File description:
** -
*/

#include "Game.hpp"
#include "GameData.hpp"
#include "Pieces/APiece.hpp"

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
    _animatedPosition.x = position.x;
    _animatedPosition.y = position.y;
    _isMoving = false;
    _sprite.setSize(CHECKFATE_TILE);
    _sprite.setFillColor(sf::Color::Red);
}

checkfate::Position APiece::getPosition(void)
{
    return _position;
}

checkfate::PositionPrecise APiece::getDisplayedPosition(void)
{
    if (_isMoving)
        return _animatedPosition;
    return checkfate::PositionPrecise(_position.x, _position.y);
}

bool APiece::display(checkfate::Game &game)
{
    sf::Vector2f spritePosition(0, 0);

    if (_isMoving) {
        spritePosition.x = _position.x;
        spritePosition.y = _position.y;
    } else {
        spritePosition.x = _animatedPosition.x;
        spritePosition.y = _animatedPosition.y;
    }
    _sprite.setPosition(spritePosition);
    game.window.draw(_sprite);
    return true;
}

};
