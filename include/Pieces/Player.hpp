/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/Player.hpp
** File description:
** -
*/

#pragma once

#include "Pieces/APiece.hpp"

namespace checkfate {

class Player : public APiece {
public:
    Player() {
        _spriteIndex = 0; _isPlayer = true; setTier(1); _name = "";
    }
    void setTier(size_t const tier) override;
};

};
