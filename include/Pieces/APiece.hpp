/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/APiece.hpp
** File description:
** -
*/

#pragma once

#include <SFML/Graphics.hpp>
#include "Pieces/IPiece.hpp"

namespace checkfate {

class Game;

class APiece : public IPiece {
protected:
    sf::RectangleShape _sprite;
    checkfate::Position _position;
    checkfate::PositionPrecise _animatedPosition;
    bool _isMoving;
public:
    void place(int const x, int const y) final;
    void place(checkfate::Position const position) override;
    //
    checkfate::Position getPosition(void) override;
    checkfate::PositionPrecise getDisplayedPosition(void) override;
    //
    bool display(checkfate::Game &game) override;
};

};
