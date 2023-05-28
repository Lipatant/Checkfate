/*
** Checkfate, Virgile (Lipatant) Berrier, 2023
** Game/Upgrade.cpp
** File description:
** -
*/

#include <vector>
#include "Game.hpp"

namespace checkfate {

static const checkfate::Upgrade upgradeList[] = {
    Upgrade("player_white_knight", "Sun Mount", \
        "[white] +Knight movement"),
    Upgrade("player_white_combo_tower", "Reflect", \
        "[white] +1 tile forwards per Combo (max 2)"),
    Upgrade("player_white_combo_bishop", "Shine", \
        "[white] +1 tile diagonally per Combo (max 2)"),
    Upgrade("player_black_knight", "Moon Mount", \
        "[black] +Knight movement"),
    Upgrade("player_black_combo_tower", "Depth", \
        "[black] +1 tile forwards per Combo (max 2)"),
    Upgrade("player_black_combo_bishop", "Twilight", \
        "[black] +1 tile diagonally per Combo (max 2)"),
    Upgrade("spawnkill", "First Strike", \
        "[any] Moving over an incomming ennemy kills it"),
    Upgrade("more_choices", "Erudite", \
        "[any] +2 choices when upgrading"),
};
static const size_t upgradeListSize = \
    sizeof(upgradeList) / sizeof(upgradeList[0]);

bool Game::newUpgrade(void)
{
    if (_scoreForUpgrade < _scorePerUpgrade) return false;
    if (gameState != checkfate::GameState::Playing) return false;
    std::vector<Upgrade> available = {};
    int randInt;

    _scoreForUpgrade -= _scorePerUpgrade;
    _ennemiesMax += 2;
    upgradesAvailable.clear();
    for (size_t i = 0; i < upgradeListSize; i++) {
        if (upgrades.has(upgradeList[i].id))
            continue;
        available.push_back(upgradeList[i]);
    }
    for (size_t i = 0; i < (upgrades.has("more_choices") ? 4 : 2); i++) {
        if (available.empty())
            break;
        randInt = std::rand() % available.size();
        upgradesAvailable.add(available[randInt]);
        available.erase(available.begin() + randInt);
    }
    if (upgradesAvailable.list.empty())
        return false;
    _updatePlayerMoves();
    gameState = checkfate::GameState::Upgrade;
    return true;
}

bool Game::_updateDisplayUIUpgrades(void)
{
    sf::Vector2f position = {_view.getSize().x / 2, _view.getSize().y * \
        static_cast<float>(0.4 - \
        (static_cast<float>(upgradesAvailable.list.size()) - 2) * 0.05)};
    sf::Text description;
    bool selected;

    _scoreText.setCharacterSize(30);
    for (auto const &upgrade : upgradesAvailable.list) {
        selected = false;
        // NAME
        _scoreText.setString(upgrade.name);
        position.x = (_view.getSize().x / 2) - \
            _scoreText.getGlobalBounds().width / 2;
        _scoreText.setPosition(position);
        if (_scoreText.getGlobalBounds().left <= _mouseUI.x && _mouseUI.x <= \
            _scoreText.getGlobalBounds().left + \
            _scoreText.getGlobalBounds().width && \
            _scoreText.getGlobalBounds().top <= _mouseUI.y && _mouseUI.y <= \
            _scoreText.getGlobalBounds().top + \
            _scoreText.getGlobalBounds().height) {
            selected = true;
        }
        position.y += _scoreText.getGlobalBounds().height;
        // DESCRIPTION
        description = _scoreText;
        description.setCharacterSize(20);
        description.setString(upgrade.description);
        position.x = (_view.getSize().x / 2) - \
            description.getGlobalBounds().width / 2;
        description.setPosition(position);
        if (selected || ( \
            description.getGlobalBounds().left <= _mouseUI.x && _mouseUI.x <= \
            description.getGlobalBounds().left + \
            description.getGlobalBounds().width && \
            description.getGlobalBounds().top <= _mouseUI.y && _mouseUI.y <= \
            description.getGlobalBounds().top + \
            description.getGlobalBounds().height)) {
            _scoreText.setFillColor(checkfate::black);
            _scoreText.setOutlineColor(checkfate::white);
            description.setFillColor(checkfate::black);
            description.setOutlineColor(checkfate::white);
            if (_mouseClickState == \
                checkfate::InputStateComplex::JustPressed) {
                upgrades.add(upgrade);
                gameState = checkfate::GameState::Playing;
                _mouseClickState = \
                    checkfate::InputStateComplex::AlreadyPressed;
                break;
            }
        } else {
            _scoreText.setFillColor(checkfate::white);
            _scoreText.setOutlineColor(checkfate::black);
            description.setFillColor(checkfate::white);
            description.setOutlineColor(checkfate::black);
        }
        window.draw(_scoreText);
        window.draw(description);
        position.y += description.getGlobalBounds().height + 20;
    }
    return true;
}

void UpgradeList::clear(void)
{
    list.clear();
}

void UpgradeList::add(Upgrade const &upgrade)
{
    if (!has(upgrade.id))
        list.push_back(upgrade);
}

bool UpgradeList::has(std::string const &upgrade) const
{
    for (auto const &element : list)
        if (element.id == upgrade)
            return true;
    return false;
}

};
