/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Piece/SetTier.cpp
** File description:
** -
*/

#include "Flags.hpp"
#include "Pieces/APiece.hpp"
#include "Pieces/Bishop.hpp"
#include "Pieces/Player.hpp"
#include "Pieces/Knight.hpp"
#include "Pieces/Queen.hpp"
#include "Pieces/Tower.hpp"

namespace checkfate {

void APiece::setTier(size_t const tier)
{
    _tier = tier; _towerLevel = tier; _bishopLevel = tier;
}

void Bishop::setTier(size_t const tier)
{
    _tier = tier; _bishopLevel = tier;
}

void Knight::setTier(FLAG_UNUSED size_t const tier)
{
    _tier = 1; _knightLevel = 1;
}

void Player::setTier(size_t const tier)
{
    _tier = tier; _towerLevel = tier; _bishopLevel = tier;
}

void Queen::setTier(size_t const tier)
{
    _tier = tier; _towerLevel = tier; _bishopLevel = tier;
}

void Tower::setTier(size_t const tier)
{
    _tier = tier; _towerLevel = tier;
}

};
