/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game.hpp
** File description:
** -
*/

#pragma once

#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameData.hpp"
#include "Pieces/Player.hpp"

namespace checkfate {

enum class InputStateComplex {
    AlreadyReleased,
    JustPressed,
    AlreadyPressed,
    JustReleased,
};

enum class GameState {
    Playing,
    Lost,
};

class Game {
private: // sprites
    sf::Color _white = checkfate::white;
    sf::Color _black = checkfate::black;
    sf::RectangleShape _chessboard;
    sf::RectangleShape _chessboardTarget;
    sf::Sprite _chessboardSprite;
    sf::Texture _chessboardTexture;
    sf::IntRect _chessboardTextureRect;
    sf::View _view;
    sf::FloatRect _viewport;
    sf::Font _font;
    bool hasFocus = true;
private:
    sf::Int64 _delta = 0;
    sf::Int64 _deltaPerUpdate = 1;
    sf::Clock _deltaClock;
    sf::Vector2f _mouseUI;
    sf::Vector2i _mouseTile;
    bool _mouseClick = false;
    checkfate::InputStateComplex _mouseClickState = \
        checkfate::InputStateComplex::AlreadyReleased;
private:
    size_t _score;
    size_t _scoreBest = 0;
    size_t _combo;
    sf::Text _scoreText;
public:
    checkfate::GameState gameState;
private:
    bool _updateWindow(void);
    bool _updateGame(void);
    bool _updateGamePlayerMoving(checkfate::Position const newPosition);
    bool _updateDisplayUI(void);
    bool _updateDisplay(void);
    bool _updateMouse(void);
    bool _updatePlayerMoves(void);
    bool _loadTextures(void);
    bool _addEnnemy(void);
public:
    sf::Texture pieceTexture;
    sf::IntRect pieceTextureRect;
    sf::Sprite pieceSprite;
    sf::Sprite pieceTierSprite;
    sf::Texture pieceTierTexture;
    sf::IntRect pieceTierTextureRect;
public:
    checkfate::Player player;
    std::list<checkfate::Piece> ennemies;
    std::list<checkfate::Piece> ennemiesIncomming;
    std::list<checkfate::Move> playerMoves;
public:
    const sf::Vector2u screenSize = {256, 224};
    sf::RenderWindow window;
public:
    Game(void);
    ~Game() = default;
    //
    bool update(void);
    bool newGame(void);
};

};
