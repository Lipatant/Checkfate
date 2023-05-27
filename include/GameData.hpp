/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** BoardData.hpp
** File description:
** -
*/

#pragma once

#include <stddef.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#define CHECKFATE_TILE {16, 16}
#define CHECKFATE_TILE_X 16
#define CHECKFATE_TILE_Y 16

namespace checkfate {

using Position = sf::Vector2i;
using PositionPrecise = sf::Vector2f;

static const sf::Color white(230, 230, 230);
static const sf::Color black(25, 25, 25);

static const size_t chessboardSizeX = 16;
static const size_t chessboardSizeY = 16;

};
