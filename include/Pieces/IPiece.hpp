/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/IPiece.hpp
** File description:
** -
*/

#pragma once

#include <list>
#include <memory>
#include "GameData.hpp"

namespace checkfate {

class Game;
class IPiece;

class Move {
public:
    checkfate::Position position;
    size_t distance = 0;
    bool isLegal = true;
    Move(checkfate::Position const position) : position(position) { }
    Move(checkfate::Position const position, size_t const distance) : \
        position(position), distance(distance) { }
    Move(checkfate::Position const position, size_t const distance, \
        bool const isLegal) : \
        position(position), distance(distance), isLegal(isLegal) { }
    //
    bool operator<(checkfate::Move const &other)
        { return distance < other.distance; }
};

class IPiece {
public:
    virtual void assignGame(Game *game) = 0;
    virtual void place(int const x, int const y) = 0;
    virtual void place(checkfate::Position const position) = 0;
    virtual void moveForce(int const x, int const y) = 0;
    virtual void moveForce(checkfate::Position const position) = 0;
    virtual void setTier(size_t const tier) = 0;
    virtual size_t getTier(bool const forDisplay) const = 0;
    virtual size_t getTier(void) const = 0;
    //
    virtual std::list<checkfate::Move> listMoves(bool const onlyLegal) = 0;
    virtual std::list<checkfate::Move> listMoves(void) = 0;
    virtual checkfate::Move bestMove(void) = 0;
    virtual bool &isWhite(void) = 0;
    virtual bool isWhite(void) const = 0;
    virtual std::string const &getName(void) const = 0;
    //
    virtual checkfate::Position getPosition(void) const = 0;
    virtual checkfate::PositionPrecise getDisplayedPosition(void) = 0;
    virtual checkfate::Position &nextMove(void) = 0;
    virtual size_t &nextMoveDelay(void) = 0;
    //
    virtual bool display(void) = 0;
};

using Piece = std::unique_ptr<checkfate::IPiece>;

template <typename T, typename... _Args>
static Piece createPiece(_Args&&... __args)
{
    return std::make_unique<T>(std::forward<_Args>(__args)...);
}

};
