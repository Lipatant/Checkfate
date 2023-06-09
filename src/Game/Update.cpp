/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game/Update.cpp
** File description:
** -
*/

#include "Flags.hpp"
#include "Game.hpp"
#include "IsValid.hpp"

#define SPLIT_V(VECTOR) VECTOR.x, VECTOR.y

#define IS_WHITE_TILE(POSITION) ((POSITION.x + POSITION.y) % 2)
#define IS_WHITE_TILE_XY(X, Y) ((X + Y) % 2)

static bool sortDisplayed(checkfate::Piece &piece1, checkfate::Piece &piece2)
{
    return piece1->getDisplayedPosition().y < piece2->getDisplayedPosition().y;
}

static sf::Vector2f getWindowRatio(sf::Window const &window)
{
    sf::Vector2u size(window.getSize());
    sf::Vector2f ratio(1, 1);

    if (size.x > size.y)
        ratio.x = (float)size.y / size.x;
    else
        ratio.y = (float)size.x / size.y;
    return ratio;
}

namespace checkfate {

bool Game::_updateWindow(void)
{
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::GainedFocus)
            hasFocus = true;
        if (event.type == sf::Event::LostFocus)
            hasFocus = false;
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();
    if (_mouseClickMState == checkfate::InputStateComplex::JustPressed) {
        _fullscreen = !_fullscreen;
        window.close();
        window.create(sf::VideoMode::getDesktopMode(), "Checkfate", \
            (_fullscreen) ? (sf::Style::Close | sf::Style::Fullscreen) : \
            (sf::Style::Close | sf::Style::Resize));
        window.setFramerateLimit(CHECKFATE_GAME_FPS);
        _mouseClickMState = checkfate::InputStateComplex::AlreadyPressed;
    }
    return true;
}

bool Game::_updatePlayerMoves(void)
{
    playerMoves = player.listMoves(false);
    return true;
}

bool Game::_updateGamePlayerMoving(checkfate::Position const newPosition)
{
    player.moveForce(newPosition);
    for (auto enemy = enemies.begin(); enemy != enemies.end();) {
        if (enemy->get()->getPosition() == newPosition) {
            addToScore(1 + combo);
            combo += 1;
            enemies.erase(enemy++);
        } else
            enemy++;
    }
    return true;
}

bool Game::_updateGame(void)
{
    bool forceUpdate = false;
    bool playerLost = false;
    size_t comboPrevious = combo;

    if (_scoreForUpgrade >= _scorePerUpgrade && newUpgrade())
        return false;
    if (_mouseClickState == checkfate::InputStateComplex::JustPressed) {
        _mouseClickState = checkfate::InputStateComplex::AlreadyPressed;
        if (isValidMouseTile(_mouseTile)) {
            for (auto const &move: playerMoves) {
                if (move.position != _mouseTile)
                    continue;
                _updateGamePlayerMoving(_mouseTile);
                forceUpdate = true;
                break;
            }
        }
    }
    if (_score > _scoreBest)
        _scoreBest = _score;
    if (forceUpdate) {
        if (comboPrevious == combo)
            combo = 0;
        if (!enemiesIncomming.empty()) {
            for (auto &enemy: enemiesIncomming) {
                if (enemy->getPosition() == player.getPosition()) {
                    if (upgrades.has("spawnkill")) {
                        addToScore(1 + combo);
                        combo += 1;
                        continue;
                    } else
                        playerLost = true;
                }
                enemies.push_back(std::move(enemy));
            }
            enemiesIncomming.push_back(checkfate::createPiece< \
                checkfate::APiece>());
            enemiesIncomming.clear();
        }
        for (auto &enemy: enemies) {
            if (enemy->nextMoveDelay() != 0)
                continue;
            enemy->moveForce(enemy->bestMove().position);
            enemy->nextMoveDelay() = 2;
            if (enemy->getPosition() == player.getPosition())
                playerLost = true;
        }
        for (auto &enemy: enemies) {
            if (enemy->nextMoveDelay() == 1)
                enemy->nextMove() = enemy->bestMove().position;
            enemy->nextMoveDelay() -= 1;
        }
        _addEnnemy();
        _updatePlayerMoves();
    }
    if (playerLost)
        gameState = checkfate::GameState::Lost;
    return true;
}

bool Game::_updateMouse(void)
{
    _mouseTile.x = _mouseUI.x / CHECKFATE_TILE_X;
    if (_mouseUI.x < 0)
        _mouseTile.x = -1;
    _mouseTile.y = _mouseUI.y / CHECKFATE_TILE_Y;
    if (_mouseUI.y < 0)
        _mouseTile.y = -1;
    if (hasFocus && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (_mouseClick)
            _mouseClickState = checkfate::InputStateComplex::AlreadyPressed;
        else
            _mouseClickState = checkfate::InputStateComplex::JustPressed;
        _mouseClick = true;
    } else {
        if (!_mouseClick)
            _mouseClickState = checkfate::InputStateComplex::AlreadyReleased;
        else
            _mouseClickState = checkfate::InputStateComplex::JustReleased;
        _mouseClick = false;
    }
    if (hasFocus && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        if (_mouseClickM)
            _mouseClickMState = checkfate::InputStateComplex::AlreadyPressed;
        else
            _mouseClickMState = checkfate::InputStateComplex::JustPressed;
        _mouseClickM = true;
    } else {
        if (!_mouseClickM)
            _mouseClickMState = checkfate::InputStateComplex::AlreadyReleased;
        else
            _mouseClickMState = checkfate::InputStateComplex::JustReleased;
        _mouseClickM = false;
    }
    return true;
}

bool Game::_updateViewMouse(void)
{
//    sf::Vector2f tileSize = _chessboard.getSize();
    sf::Vector2f windowRatio(getWindowRatio(window));
    checkfate::PositionPrecise playerDisplayed(player.getDisplayedPosition());
    sf::Vector2i positionPixels = sf::Mouse::getPosition(window);
    sf::Vector2f halfScreen;

    playerDisplayed.x += static_cast<float>(pieceTextureRect.width) / 2;
    _view = window.getView();
    _view.setSize(screenSize.x, screenSize.y);
    _viewport = sf::FloatRect(1, 1, windowRatio.x, windowRatio.y);
    _viewport.left = (1 - _viewport.width) / 2;
    _viewport.top = (1 - _viewport.height) / 2;
    _view.setCenter(0, 0);
    _view.setViewport(_viewport);
    _mouseUI = window.mapPixelToCoords(positionPixels, _view);
    //
    halfScreen.x = static_cast<float>(screenSize.x) / 2;
    halfScreen.y = static_cast<float>(screenSize.y) / 2;
    if (_mouseUI.x < -halfScreen.x) _mouseUI.x = -halfScreen.x;
    if (_mouseUI.y < -halfScreen.y) _mouseUI.y = -halfScreen.y;
    if (_mouseUI.x > halfScreen.x) _mouseUI.x = halfScreen.x;
    if (_mouseUI.y > halfScreen.y) _mouseUI.y = halfScreen.y;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (halfScreen.x != 0)
            playerDisplayed.x += (1 - (halfScreen.x - _mouseUI.x) \
                / halfScreen.x) * CHECKFATE_TILE_X * 2;
        if (halfScreen.y != 0)
            playerDisplayed.y += (1 - (halfScreen.y - _mouseUI.y) \
                / halfScreen.y) * CHECKFATE_TILE_Y * 2;
    }
    _view.setCenter(playerDisplayed);
    _mouseUI = window.mapPixelToCoords(positionPixels, _view);
    _updateMouse();
    window.setView(_view);
    return true;
}

bool Game::_updateDisplayUI(void)
{
    size_t nextUpgrade = 0;

    _view = window.getView();
    _view.setCenter({static_cast<float>(screenSize.x) * 2, \
        static_cast<float>(screenSize.y) * 2});
    _view.setSize(screenSize.x * 4, screenSize.y * 4);
    _scoreText.setFillColor(checkfate::white);
    _scoreText.setOutlineColor(checkfate::black);
    _mouseUI = window.mapPixelToCoords(sf::Mouse::getPosition(window), _view);
    window.setView(_view);
    if (_scoreForUpgrade < _scorePerUpgrade)
        nextUpgrade = _scorePerUpgrade - _scoreForUpgrade;
    _scoreText.setCharacterSize(30);
    _scoreText.setString("Next upgrade:");
    _scoreText.setPosition({_view.getSize().x - \
        _scoreText.getLocalBounds().width - 20, 20});
    window.draw(_scoreText);
    _scoreText.setCharacterSize(40);
    float storedY = _scoreText.getPosition().y + \
        _scoreText.getGlobalBounds().height;
    _scoreText.setString(std::to_string(nextUpgrade));
    _scoreText.setPosition({_view.getSize().x - \
        _scoreText.getLocalBounds().width - 20, storedY});
    window.draw(_scoreText);
    // Unit
    if (!_selectedPieceName.empty() && gameState == \
        checkfate::GameState::Playing) {
        _scoreText.setCharacterSize(30);
        storedY = _scoreText.getPosition().y + \
            _scoreText.getGlobalBounds().height + 20;
        if (_selectedPieceTier > 0)
            _scoreText.setString(_selectedPieceName + " " + \
                std::to_string(_selectedPieceTier));
        else
            _scoreText.setString(_selectedPieceName);
        _scoreText.setPosition({_view.getSize().x - \
            _scoreText.getLocalBounds().width - 20, storedY});
        window.draw(_scoreText);
    }
    // Score
    _scoreText.setCharacterSize(40);
    _scoreText.setPosition({20, 20});
    _scoreText.setString("Score: " + std::to_string(_score));
    window.draw(_scoreText);
    _scoreText.setPosition({20, _scoreText.getPosition().y + \
        _scoreText.getLocalBounds().height + 20});
    _scoreText.setString("Combo: " + std::to_string(combo));
    window.draw(_scoreText);
    _scoreText.setString("Best: " + std::to_string(_scoreBest));
    _scoreText.setPosition({20, _view.getSize().y - \
        _scoreText.getLocalBounds().height - 20});
    window.draw(_scoreText);
    if (gameState == checkfate::GameState::Upgrade)
        _updateDisplayUIUpgrades();
    if (gameState != checkfate::GameState::Lost)
        return true;
    _scoreText.setString("Leave");
    _scoreText.setPosition({_view.getSize().x - \
        _scoreText.getLocalBounds().width - 20, _view.getSize().y - \
        _scoreText.getLocalBounds().height - 20});
    if (_scoreText.getGlobalBounds().left <= _mouseUI.x && _mouseUI.x <= \
        _scoreText.getGlobalBounds().left + \
        _scoreText.getGlobalBounds().width && \
        _scoreText.getGlobalBounds().top <= _mouseUI.y && _mouseUI.y <= \
        _scoreText.getGlobalBounds().top + \
        _scoreText.getGlobalBounds().height) {
        _scoreText.setFillColor(checkfate::black);
        _scoreText.setOutlineColor(checkfate::white);
        if (_mouseClickState == checkfate::InputStateComplex::JustPressed) {
            window.close();
            _mouseClickState = checkfate::InputStateComplex::AlreadyPressed;
        }
    }
    window.draw(_scoreText);
    _scoreText.setFillColor(checkfate::white);
    _scoreText.setOutlineColor(checkfate::black);
    _scoreText.setString("Retry?");
    _scoreText.setPosition({_view.getSize().x - \
        _scoreText.getLocalBounds().width - 20, _scoreText.getPosition().y - \
        _scoreText.getLocalBounds().height - 20});
    if (_scoreText.getGlobalBounds().left <= _mouseUI.x && _mouseUI.x <= \
        _scoreText.getGlobalBounds().left + \
        _scoreText.getGlobalBounds().width && \
        _scoreText.getGlobalBounds().top <= _mouseUI.y && _mouseUI.y <= \
        _scoreText.getGlobalBounds().top + \
        _scoreText.getGlobalBounds().height) {
        _scoreText.setFillColor(checkfate::black);
        _scoreText.setOutlineColor(checkfate::white);
        if (_mouseClickState == checkfate::InputStateComplex::JustPressed) {
            newGame();
            _mouseClickState = checkfate::InputStateComplex::AlreadyPressed;
        }
    }
    window.draw(_scoreText);
    return true;
}

bool Game::_updateDisplayEnnemyTarget(checkfate::Position const position)
{
    sf::Color color = checkfate::red;
    sf::Color baseColor = IS_WHITE_TILE(position) ? \
        checkfate::white : checkfate::black;
    bool isTransparent = false;

    if (position != player.getPosition()) {
        isTransparent = true;
        for (auto &move: playerMoves) {
            if (move.position != position)
                continue;
            isTransparent = false;
            break;
        }
    }
    if (isTransparent) {
        color.r = (static_cast<short>(color.r) + baseColor.r) / 2;
        color.g = (static_cast<short>(color.g) + baseColor.g) / 2;
        color.b = (static_cast<short>(color.b) + baseColor.b) / 2;
    }
    _chessboardSprite.setColor(color);
    _chessboardSprite.setPosition(position.x * \
        _chessboard.getSize().x, position.y * _chessboard.getSize().y);
    window.draw(_chessboardSprite);
    _chessboardSprite.setColor(checkfate::red);
    return true;
}

bool Game::_updateDisplayEnnemyTarget(checkfate::Position const position, \
    bool const isTransparent)
{
    sf::Color color = checkfate::red;
    sf::Color baseColor = IS_WHITE_TILE(position) ? \
        checkfate::white : checkfate::black;

    if (isTransparent) {
        color.r = (static_cast<short>(color.r) + baseColor.r) / 2;
        color.g = (static_cast<short>(color.g) + baseColor.g) / 2;
        color.b = (static_cast<short>(color.b) + baseColor.b) / 2;
    }
    _chessboardSprite.setColor(color);
    _chessboardSprite.setPosition(position.x * \
        _chessboard.getSize().x, position.y * _chessboard.getSize().y);
    window.draw(_chessboardSprite);
    _chessboardSprite.setColor(checkfate::red);
    return true;
}

bool Game::_updateDisplayEnnemyTarget(checkfate::Position const position, \
    FLAG_UNUSED checkfate::Piece const &enemy)
{
    sf::Color color = checkfate::red;
    sf::Color baseColor = IS_WHITE_TILE(position) ? \
        checkfate::white : checkfate::black;
    bool isTransparent = false;

    if (enemy->nextMoveDelay() != 0)
        isTransparent = true;
    else if (position != player.getPosition()) {
        isTransparent = true;
        for (auto &move: playerMoves) {
            if (move.position != position)
                continue;
            isTransparent = false;
            break;
        }
    }
    if (isTransparent) {
        color.r = (static_cast<short>(color.r) + baseColor.r) / 2;
        color.g = (static_cast<short>(color.g) + baseColor.g) / 2;
        color.b = (static_cast<short>(color.b) + baseColor.b) / 2;
    }
    _chessboardSprite.setColor(color);
    _chessboardSprite.setPosition(position.x * \
        _chessboard.getSize().x, position.y * _chessboard.getSize().y);
    window.draw(_chessboardSprite);
    _chessboardSprite.setColor(checkfate::red);
    return true;
}

bool Game::_updateDisplay(void)
{
    sf::Vector2f tileSize = _chessboard.getSize();
    bool const hasSpawnkill = upgrades.has("spawnkill");
//    bool const hideOtherEnemies = true;

    _updateViewMouse();
    window.clear(sf::Color::Black);
    for (size_t x = 0; x < checkfate::chessboardSizeX; x++) {
        for (size_t y = 0; y < checkfate::chessboardSizeY; y++) {
            if (IS_WHITE_TILE_XY(x, y))
                _chessboard.setFillColor(_white);
            else
                _chessboard.setFillColor(_black);
            _chessboard.setPosition(x * tileSize.x, y * tileSize.y);
            window.draw(_chessboard);
        }
    }
    if (gameState == GameState::Playing) {
        if (_mouseTile == player.getPosition())
            _selectedPieceName = player.getName();
        else
            _selectedPieceName = "";
        _selectedPieceTier = 0;
        _chessboardTarget.setOutlineColor(checkfate::blue);
        _chessboardSprite.setColor(checkfate::blue);
        _chessboardTextureRect.left = CHECKFATE_TILE_X * 1;
        _chessboardSprite.setTextureRect(_chessboardTextureRect);
        for (auto const move: playerMoves) {
            _chessboardTarget.setPosition(move.position.x * tileSize.x + 2, \
                move.position.y * tileSize.y + 2);
            window.draw(_chessboardTarget);
            _chessboardSprite.setPosition(move.position.x * tileSize.x, \
                move.position.y * tileSize.y);
            for (auto const &enemy: enemies) {
                if (move.position == enemy->getPosition()) {
                    window.draw(_chessboardSprite);
                    break;
                }
            }
            if (hasSpawnkill) {
                for (auto const &enemy: enemiesIncomming) {
                    if (move.position == enemy->getPosition()) {
                        window.draw(_chessboardSprite);
                        break;
                    }
                }
            }
        }
        _chessboardTarget.setOutlineColor(checkfate::green);
        _chessboardSprite.setColor(checkfate::red);
        _chessboardTextureRect.left = CHECKFATE_TILE_Y * 0;
        _chessboardSprite.setTextureRect(_chessboardTextureRect);
        /// Ennemies Incomming
        for (auto &enemy: enemiesIncomming) {
            _chessboardTarget.setPosition(enemy->getPosition().x * \
                tileSize.x + 2, enemy->getPosition().y * tileSize.y + 2);
            window.draw(_chessboardTarget);
            if (hasSpawnkill)
                continue;
            _updateDisplayEnnemyTarget(enemy->getPosition());
        }
        _chessboardTarget.setOutlineColor(checkfate::red);
        /// Ennemy Next Move
        for (auto &enemy: enemies) {
            if (enemy->nextMoveDelay() != 0)
                continue;
            _chessboardTarget.setPosition(enemy->getPosition().x * \
                tileSize.x + 2, enemy->getPosition().y * tileSize.y + 2);
            window.draw(_chessboardTarget);
            _updateDisplayEnnemyTarget(enemy->nextMove(), enemy);
        }
        for (auto &enemy: enemies) {
            if (_mouseTile != enemy->getPosition())
                continue;
            for (auto const &move: enemy->listMoves(false))
                _updateDisplayEnnemyTarget(move.position, enemy);
            _selectedPieceName = enemy->getName();
            _selectedPieceTier = enemy->getTier();
            break;
        }
    }
    enemies.sort(sortDisplayed);
    bool playerHasBeenDisplayed = false;
    for (auto &enemy: enemies) {
        if (!playerHasBeenDisplayed && enemy->getDisplayedPosition().y >= \
            player.getDisplayedPosition().y) {
            player.display();
            playerHasBeenDisplayed = true;
        }
        enemy->display();
    }
    if (!playerHasBeenDisplayed)
        player.display();
    _updateDisplayUI();
    window.display();
    return true;
}

bool Game::update(void)
{
    _delta += _deltaClock.getElapsedTime().asMicroseconds();
    _updateWindow();
    _deltaClock.restart();
    while (_delta >= _deltaPerUpdate) {
        if (gameState == checkfate::GameState::Playing)
            _updateGame();
        _delta -= _deltaPerUpdate;
    }
    _updateDisplay();
    return window.isOpen();
}

};
