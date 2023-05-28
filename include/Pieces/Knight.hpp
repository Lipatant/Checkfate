/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/Knight.hpp
** File description:
** -
*/

#pragma once

#include "Pieces/APiece.hpp"

namespace checkfate {

class Knight : public APiece {
public:
    Knight(size_t const tier) {
        _spriteIndex = 4; setTier(tier);
    }
    void setTier(size_t const tier) override;
};

};
