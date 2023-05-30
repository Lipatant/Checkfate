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
    Upgrade("W_player_knight", "Sun Mount", \
        "[white] +Knight movement"),
    Upgrade("W_player_combo_tower", "Reflect", \
        "[white] +1 tile forwards per Combo above 1 (max +2)"),
    Upgrade("W_player_combo_bishop", "Shine", \
        "[white] +1 tile diagonally per Combo above 1 (max +2)"),
    Upgrade("B_player_knight", "Moon Mount", \
        "[black] +Knight movement"),
    Upgrade("B_player_combo_tower", "Depth", \
        "[black] +1 tile forwards per Combo above 1 (max +2)"),
    Upgrade("B_player_combo_bishop", "Twilight", \
        "[black] +1 tile diagonally per Combo above 1 (max +2)"),
    Upgrade("spawnkill", "First Strike", \
        "[any] Moving over an incomming ennemy kills it"),
    Upgrade("more_choices", "Erudite", \
        "[any] +2 choices when upgrading (includes challenges)"),
    Upgrade("player_not_moving", "Inner Peace", \
        "[any] You can move on yourself if you have at least 1 Combo"),
};

static const checkfate::Upgrade challengeList[] = {
    Upgrade("challenge_stronger_tower", "Watchtowers", \
        "[any] Rooks spawns with +1 tier"),
    Upgrade("challenge_hidden_opposite", "Fog", \
        "[any] Hides the tier of ennemies of opposide color"),
};

static const size_t upgradeListSize = \
    sizeof(upgradeList) / sizeof(upgradeList[0]);
static const size_t challengeListSize = \
    sizeof(challengeList) / sizeof(challengeList[0]);

bool Game::_chooseUpgrade(bool const challenge)
{
    std::vector<Upgrade> available = {};
    int randInt;

    upgradesAvailable.clear();
    if (challenge) {
        for (size_t i = 0; i < challengeListSize; i++) {
            if (upgrades.has(challengeList[i].id))
                continue;
            available.push_back(challengeList[i]);
        }
    } else {
        for (size_t i = 0; i < upgradeListSize; i++) {
            if (upgrades.has(upgradeList[i].id))
                continue;
            available.push_back(upgradeList[i]);
        }
    }
    for (size_t i = 0; i < (upgrades.has("more_choices") ? 4 : 2); i++) {
        if (available.empty())
            break;
        randInt = std::rand() % available.size();
        upgradesAvailable.add(available[randInt]);
        available.erase(available.begin() + randInt);
    }
    return !upgradesAvailable.list.empty();
}

bool Game::newUpgrade(void)
{
    if (_scoreForUpgrade < _scorePerUpgrade) return false;
    if (gameState != checkfate::GameState::Playing) return false;

    _scoreForUpgrade -= _scorePerUpgrade;
    if (_ennemiesTier < 4)
        _ennemiesTier++;
    _ennemiesMax += 2;
    if (!_chooseUpgrade(false))
        if (!_chooseUpgrade(true))
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
        description.setCharacterSize(16);
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
                _updatePlayerMoves();
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

void UpgradeList::add(std::string const &upgrade)
{
    if (!has(upgrade))
        list.push_back(checkfate::Upgrade(upgrade));
}

bool UpgradeList::has(std::string const &upgrade) const
{
    for (auto const &element : list)
        if (element.id == upgrade)
            return true;
    return false;
}

};
