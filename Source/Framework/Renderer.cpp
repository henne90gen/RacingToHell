//
// Created by henne on 27.11.16.
//

#include "stdafx.h"
#include "Framework/Renderer.h"
#include "Framework/Framework.h"

Renderer::Renderer(Framework &framework) : _FW(framework) {

}

void Renderer::render() {
    if ((_FW.getGameState() != GameState::Loading /*|| _CurrentGameScreen.isFadingAway()*/) &&
        _FW.getGameState() != GameState::Countdown) {

//        _FW.getLevel().render(_RenderWindow);

        if (_FW.getNetworkHandle().getRelation() == NetworkRelation::NoRel) {
            // FIXME reenable goc
//            _FW.getGOC().render(_RenderWindow,
//                                _FW.getGameState() == GameState::Running || _FW.getGameState() == GameState::BossFight);
        } else {
//            _FW.getMPGOC().render(_RenderWindow,
//                                  _FW.getGameState() == GameState::RunningMultiplayer ||
//                                  _FW.getGameState() == GameState::BossFight);
        }
    }

//    setMouseVisibility();

//    for (int i = 0; i < _DisplayedGameScreens.size(); i++) {
//        _DisplayedGameScreens.at(i)->render(_RenderWindow);
//    }

    /*switch (_FW.getGameState()) {
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
    }*/
//    _RenderWindow.display();
}

