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

#define CHECKFATE_GAME_FPS 60
#define CHECKFATE_TILE {24, 24}
#define CHECKFATE_TILE_X 24
#define CHECKFATE_TILE_Y 24
#define CHECKFATE_CHESSBOARD_SIZE {16, 16}
#define CHECKFATE_CHESSBOARD_SIZE_X 16
#define CHECKFATE_CHESSBOARD_SIZE_Y 16

namespace checkfate {

using Position = sf::Vector2i;
using PositionPrecise = sf::Vector2f;

static const sf::Color white(230, 230, 230);
static const sf::Color black(25, 25, 25);
static const sf::Color red(white.r, black.g, black.b);
static const sf::Color green(black.r, white.g, black.b);
static const sf::Color blue(black.r, black.g, white.b);

static const size_t chessboardSizeX = CHECKFATE_CHESSBOARD_SIZE_X;
static const size_t chessboardSizeY = CHECKFATE_CHESSBOARD_SIZE_Y;

};
