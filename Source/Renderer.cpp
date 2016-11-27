//
// Created by henne on 27.11.16.
//

#include "Renderer.h"
#include "Framework.hpp"

Renderer::Renderer(Framework &framework) : _FW(framework) {
    _RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell", sf::Style::Close);

#ifdef SFML_SYSTEM_WINDOWS
    _RenderWindow.setMouseCursorVisible(false);
#else
    _RenderWindow.setMouseCursorVisible(true);
#endif

    sf::Image Icon;
    if (Icon.loadFromFile("Resources/Texture/Icon/Icon.png")) {
        _RenderWindow.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
    }
}

void Renderer::render() {
    if ((_FW.getGameState() != GameState::Loading || _FW.getLoadingScreen().isFadingAway()) &&
        _FW.getGameState() != GameState::Countdown) {
        _FW.getLevel().render(_RenderWindow);

        if (_FW.getNetworkHandle().getRelation() == NetworkRelation::NoRel) {
            _FW.getGOC().render(_RenderWindow,
                                _FW.getGameState() == GameState::Running || _FW.getGameState() == GameState::BossFight);
        } else {
            _FW.getMPGOC().render(_RenderWindow,
                                  _FW.getGameState() == GameState::RunningMultiplayer ||
                                  _FW.getGameState() == GameState::BossFight);
        }
    }

    setMouseVisibility();

    switch (_FW.getGameState()) {
        case GameState::Running:
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::Main:
            _FW.getMainMenu().render(_RenderWindow);
            break;
        case GameState::Pause:
            _FW.getPauseMenu().render(_RenderWindow);
            break;
        case GameState::Highscores:
            _FW.getHighscoreMenu().render(_RenderWindow);
            break;
        case GameState::Options:
            _FW.getOptionsMenu().render(_RenderWindow);
            break;
        case GameState::About:
            _FW.getAboutScreen().render(_RenderWindow);
            break;
        case GameState::LevelUp:
            _FW.getLevelUpScreen().render(_RenderWindow);
            break;
        case GameState::BossFight:
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::GameOver:
            _FW.getGameOverScreen().render(_RenderWindow);
            break;
        case GameState::Loading:
            if (_FW.getLoadingScreen().isFadingAway()) {
                _FW.getMainMenu().render(_RenderWindow);
            }
            _FW.getLoadingScreen().render(_RenderWindow);
            break;
        case GameState::MultiplayerSelection:
            _FW.getMultiplayerMenu().render(_RenderWindow);
            break;
        case GameState::Connecting:
            _FW.getMultiplayerMenu().render(_RenderWindow);
            break;
        case GameState::Lobby:
            _FW.getMultiplayerLobby().render(_RenderWindow);
            break;
        case GameState::Countdown:
            _FW.getCountdown().render(_RenderWindow);
            break;
        case GameState::RunningMultiplayer:
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::PauseMultiplayer:
            _FW.getPauseMultiplayerMenu().render(_RenderWindow);
            break;
        case GameState::Exiting:
            break;
        case GameState::GameOverMultiplayer:
            break;
        case GameState::BossFightMultiplayer:
            break;
    }
    _RenderWindow.display();
}

void Renderer::setMouseVisibility() {
    bool visible;
    switch (_FW.getGameState()) {
        case GameState::PauseMultiplayer:
        case GameState::Lobby:
        case GameState::Connecting:
        case GameState::MultiplayerSelection:
        case GameState::GameOver:
        case GameState::About:
        case GameState::Options:
        case GameState::Highscores:
        case GameState::Pause:
        case GameState::Main:
            visible = true;
            break;
        case GameState::Running:
        case GameState::RunningMultiplayer:
        case GameState::Countdown:
        case GameState::Loading:
        case GameState::BossFight:
        case GameState::LevelUp:
            visible = true;
            break;
        case GameState::Exiting:
            break;
        case GameState::GameOverMultiplayer:
            break;
        case GameState::BossFightMultiplayer:
            break;
    }

#ifdef SFML_SYSTEM_WINDOWS
    if (!visible) {
        int cursor = ShowCursor(0);
        while (cursor > 0) {
            cursor = ShowCursor(0);
        }
    }
    else {
        int cursor = ShowCursor(1);
        while (cursor < 0) {
            cursor = ShowCursor(1);
        }
    }
#else
    _RenderWindow.setMouseCursorVisible(visible);
#endif
}

