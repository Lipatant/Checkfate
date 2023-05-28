/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/Queen.hpp
** File description:
** -
*/

#pragma once

#include "Pieces/APiece.hpp"

namespace checkfate {

class Queen : public APiece {
public:
    Queen(size_t const tier) {
        _spriteIndex = 2; setTier(tier);
    }
    void setTier(size_t const tier) override;
};

};
