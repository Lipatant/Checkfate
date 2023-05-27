/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Main.cpp
** File description:
** -
*/

#include "Error.hpp"
#include "Game.hpp"

int main(void)
{
    checkfate::Game game;

    if (!game.window.isOpen())
        return error::raiseInt("Couldn't open an sf::Window");
    game.newGame();
    while (game.update());
    return 0;
}
