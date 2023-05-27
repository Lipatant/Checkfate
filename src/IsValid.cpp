/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** IsValid.cpp
** File description:
** -
*/

#include "IsValid.hpp"
#include "GameData.hpp"

bool isValidMouseTile(sf::Vector2i const mouseTile)
{
    return (mouseTile.x >= 0 && mouseTile.y >= 0 && mouseTile.x < \
        CHECKFATE_CHESSBOARD_SIZE_X && mouseTile.y < \
        CHECKFATE_CHESSBOARD_SIZE_Y);
}

bool isValidMouseTile(int const x, int const y)
{
    return (x >= 0 && y >= 0 && x < CHECKFATE_CHESSBOARD_SIZE_X && y < \
        CHECKFATE_CHESSBOARD_SIZE_Y);
}
