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
    checkfate::Position _positionPrevious;
    checkfate::Position _position;
    checkfate::PositionPrecise _animatedPosition;
    sf::Int32 _animationSpeed = 1;
    size_t _spriteIndex = 0;
    size_t _bishopLevel = 0;
    size_t _rookLevel = 0;
    size_t _knightLevel = 0;
    size_t _tier = 0;
    sf::Clock _clock;
    bool _isMoving = false;
    bool _displayed = false;
    Game *_game = 0;
    size_t _nextMoveDelay = 0;
    bool _isPlayer = false;
    checkfate::Position _nextMove;
    bool _isWhite = false;
    bool _isWhitePrevious = false;
    std::string _name = std::string("Piece");
    bool _hideTier = false;
protected:
    sf::Color _colorTransition(sf::Color const pri, sf::Color const sec);
    bool _addMovement(std::list<checkfate::Move> &moves, checkfate::Move \
        move, bool const onlyLegal);
public:
    void assignGame(Game *game) final;
    void place(int const x, int const y) final;
    void place(checkfate::Position const position) override;
    void moveForce(int const x, int const y) final;
    void moveForce(checkfate::Position const position) override;
    void setTier(size_t const tier) override;
    size_t getTier(bool const forDisplay) const override;
    size_t getTier(void) const final;
    bool &isWhite(void) final;
    bool isWhite(void) const final;
    //
    /// @param onlyLegal (bool const) true by default
    std::list<checkfate::Move> listMoves(bool const onlyLegal) override;
    /// @param onlyLegal (bool const) true by default
    std::list<checkfate::Move> listMoves(void) override;
    checkfate::Move bestMove(void) override;
    std::string const &getName(void) const override;
    //
    checkfate::Position getPosition(void) const override;
    checkfate::PositionPrecise getDisplayedPosition(void) override;
    checkfate::Position &nextMove(void) override;
    size_t &nextMoveDelay(void) override;
    //
    bool display(void) override;
};

};
