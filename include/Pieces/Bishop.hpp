/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/Bishop.hpp
** File description:
** -
*/

#pragma once

#include "Pieces/APiece.hpp"

namespace checkfate {

class Bishop : public APiece {
public:
    Bishop(size_t const tier) {
        _spriteIndex = 3; setTier(tier); _name = "Bishop";
    }
    void setTier(size_t const tier) override;
};

};
