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

//        if (_NetworkHandle.getRelation() == NetworkRelation::NoRel) {
        _FW.getGOC().render(_RenderWindow,
                            _FW.getGameState() == GameState::Running || _FW.getGameState() == GameState::BossFight);
//        } else {
//            _MPGOCClient.render(_RenderWindow,
//                                _GameState == GameState::RunningMultiplayer || _GameState == GameState::BossFight);
//        }
    }

    switch (_FW.getGameState()) {
        case GameState::Running:
            setMouseVisible(false);
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::Main:
            setMouseVisible(true);
            _FW.getMainMenu().render(_RenderWindow);
            break;
        case GameState::Pause:
            setMouseVisible(true);
            _FW.getPauseMenu().render(_RenderWindow);
            break;
        case GameState::Highscores:
            setMouseVisible(true);
            _FW.getHighscoreMenu().render(_RenderWindow);
            break;
        case GameState::Options:
            setMouseVisible(true);
            _FW.getOptionsMenu().render(_RenderWindow);
            break;
        case GameState::About:
            setMouseVisible(true);
            _FW.getAboutScreen().render(_RenderWindow);
            break;
        case GameState::LevelUp:
            setMouseVisible(false);
            _FW.getLevelUpScreen().render(_RenderWindow);
            break;
        case GameState::BossFight:
            setMouseVisible(false);
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::GameOver:
            setMouseVisible(true);
            _FW.getGameOverScreen().render(_RenderWindow);
            break;
        case GameState::Loading:
            setMouseVisible(false);
            if (_FW.getLoadingScreen().isFadingAway()) {
                _FW.getMainMenu().render(_RenderWindow);
            }
            _FW.getLoadingScreen().render(_RenderWindow);
            break;
        case GameState::MultiplayerSelection:
            setMouseVisible(true);
            _FW.getMultiplayerMenu().render(_RenderWindow);
            break;
        case GameState::Connecting:
            setMouseVisible(true);
            _FW.getMultiplayerMenu().render(_RenderWindow);
            break;
        case GameState::Lobby:
            setMouseVisible(true);
            _FW.getMultiplayerLobby().render(_RenderWindow);
            break;
        case GameState::Countdown:
            setMouseVisible(false);
            _FW.getCountdown().render(_RenderWindow);
            break;
        case GameState::RunningMultiplayer:
            setMouseVisible(false);
            _FW.getHUD().render(_RenderWindow);
            break;
        case GameState::PauseMultiplayer:
            setMouseVisible(true);
            _FW.getPauseMultiplayerMenu().render(_RenderWindow);
            break;
    }
    _RenderWindow.display();
}

void Renderer::setMouseVisible(bool visible) {
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

