/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/Tower.hpp
** File description:
** -
*/

#pragma once

#include "Pieces/APiece.hpp"

namespace checkfate {

class Tower : public APiece {
public:
    Tower(size_t const tier) {
        _spriteIndex = 5; setTier(tier); _name = "Rook";
    }
    void setTier(size_t const tier) override;
};

};
