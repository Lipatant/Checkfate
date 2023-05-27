/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Main.cpp
** File description:
** -
*/

#include <iostream>

namespace error {

int raiseInt(std::string const &msg)
{
    std::cerr << "Error: " << msg << std::endl;
    return 84;
}

bool raiseBool(std::string const &msg)
{
    std::cerr << "Error: " << msg << std::endl;
    return false;
}

}
