/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/IPiece.hpp
** File description:
** -
*/

#pragma once

#include "GameData.hpp"

namespace checkfate {

class Game;

class IPiece {
public:
    virtual void place(int const x, int const y) = 0;
    virtual void place(checkfate::Position const position) = 0;
    virtual void moveForce(int const x, int const y) = 0;
    virtual void moveForce(checkfate::Position const position) = 0;
    //
    virtual checkfate::Position getPosition(void) = 0;
    virtual checkfate::PositionPrecise getDisplayedPosition(void) = 0;
    //
    virtual bool display(checkfate::Game &game) = 0;
};

};
