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
    Upgrade,
    Lost,
};

class Upgrade {
public:
    std::string id;
    std::string name;
    std::string description;
public:
    Upgrade(std::string const &id, std::string const &name, \
        std::string const &description) : \
        id(id), name(name), description(description) {}
    Upgrade(std::string const &id) : \
        id(id), name(id), description("???") {}
    Upgrade(Upgrade const &other) : \
        id(other.id), name(other.name), description(other.description) {}
};

class UpgradeList {
public:
    std::list<Upgrade> list;
public:
    void clear(void);
    void add(Upgrade const &upgrade);
    void add(std::string const &upgrade);
    bool has(std::string const &upgrade) const;
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
    bool _mouseClickM = false;
    bool _fullscreen = false;
    checkfate::InputStateComplex _mouseClickState = \
        checkfate::InputStateComplex::AlreadyReleased;
    checkfate::InputStateComplex _mouseClickMState = \
        checkfate::InputStateComplex::AlreadyReleased;
private:
    size_t _score;
    size_t _scoreBest = 0;
    size_t _scorePerUpgrade = 25; //25;
    size_t _scoreForUpgrade;
    size_t _ennemiesMax;
    size_t _ennemiesTier;
    sf::Text _scoreText;
    std::string _selectedPieceName;
    size_t _selectedPieceTier;
public:
    size_t combo;
    checkfate::GameState gameState;
    checkfate::UpgradeList upgrades;
    checkfate::UpgradeList upgradesAvailable;
private:
    bool _updateWindow(void);
    bool _updateGame(void);
    bool _updateGamePlayerMoving(checkfate::Position const newPosition);
    bool _updateDisplayUI(void);
    bool _updateDisplayUIUpgrades(void);
    bool _updateDisplay(void);
    bool _updateMouse(void);
    bool _updatePlayerMoves(void);
    bool _loadTextures(void);
    bool _addEnnemy(void);
    bool _chooseUpgrade(bool const challenge);
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
    void addToScore(size_t const amount);
    bool newUpgrade(void);
};

};
