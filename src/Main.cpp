/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Main.cpp
** File description:
** -
*/

#include "Error.hpp"
#include "Game.hpp"

static const size_t SCREEN_X = 256;
static const size_t SCREEN_Y = 224;

int main(void)
{
    checkfate::Game game;

    if (!game.window.isOpen())
        return error::raiseInt("Couldn't open an sf::Window");
    while (game.update());
    return 0;
}
