/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/IPiece.hpp
** File description:
** -
*/

#pragma once

#include <list>
#include "GameData.hpp"

namespace checkfate {

class Game;
class IPiece;

class Move {
public:
    checkfate::Position position;
    bool inCheck = false;
    Move(checkfate::Position const position) : position(position) { }
    Move(checkfate::Position const position, bool const inCheck) : \
        position(position), inCheck(inCheck) { }
};

class IPiece {
public:
    virtual void place(int const x, int const y) = 0;
    virtual void place(checkfate::Position const position) = 0;
    virtual void moveForce(int const x, int const y) = 0;
    virtual void moveForce(checkfate::Position const position) = 0;
    //
    virtual std::list<checkfate::Move> listMoves(bool const onlyLegal) = 0;
    virtual std::list<checkfate::Move> listMoves(void) = 0;
    virtual checkfate::Move bestMove(void) = 0;
    //
    virtual checkfate::Position getPosition(void) = 0;
    virtual checkfate::PositionPrecise getDisplayedPosition(void) = 0;
    //
    virtual bool display(checkfate::Game &game) = 0;
};

};
