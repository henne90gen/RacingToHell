//
// Created by henne on 27.11.16.
//

#include "Framework.hpp"
#include "EventHandler.h"

EventHandler::EventHandler(Framework &framework) : _FW(framework) {

}

void EventHandler::handleEvents(sf::RenderWindow &renderWindow) {
    sf::Event event;
    switch (_FW.getGameState()) {
        case GameState::Running:
            if (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _FW.setGameState(GameState::Exiting);
                } else if (event.type == sf::Event::MouseLeft || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
                           sf::Joystick::isButtonPressed(0, 7)) {
                    _FW.getPauseMenu().setReturnState(_FW.getGameState());
                    _FW.getGameState() = GameState::Pause;
                } else {
                    _FW.getGOC().handleEvent(event);
                }
            }
            break;
        case GameState::Pause:
            _FW.getGOC().stopSounds();
            _FW.getGameState() = _FW.getPauseMenu().handleEvents(renderWindow);
            if (_FW.getGameState() == GameState::Running) {
                _FW.restartClock();
            } else if (_FW.getGameState() == GameState::Main) {
                _FW.resetGame();
            } else if (_FW.getGameState() == GameState::Options) {
                _FW.getOptionsMenu().enableDifficultySelection(false);
                _FW.getOptionsMenu().enableGameModeSelection(false);
                _FW.getOptionsMenu().setReturnState(GameState::Pause);
            }
            break;
        case GameState::Main:
            _FW.getGameState() = _FW.getMainMenu().handleEvents(renderWindow);

            _FW.setCurrentCarSkinIndex(_FW.getMainMenu().getCarIndex());
            if (_FW.getCurrentCarSkinIndex() < 0) {
                _FW.setCurrentCarSkinIndex((int) (_FW.getNumberOfCarsAvailable() - 1));
            } else if (_FW.getCurrentCarSkinIndex() >= _FW.getNumberOfCarsAvailable()) {
                _FW.setCurrentCarSkinIndex(0);
            }
            _FW.getMainMenu().setCarIndex(_FW.getCurrentCarSkinIndex());

            _FW.updateCarSelection();

            _FW.setGameMode(_FW.getOptionsMenu().getGameMode());

            if (_FW.getGameMode() == GameMode::Hardcore) {
                _FW.getGOC().getPlayerCar()->setMaxHealth(15);
                _FW.getGOC().getPlayerCar()->setHealth(15);
            }

            if (_FW.getGameState() == GameState::Running) {
                _FW.getHUD().setMaxHealth(_FW.getGOC().getPlayerCar()->getMaxHealth());
                _FW.getHUD().setMaxEnergy(_FW.getGOC().getPlayerCar()->getMaxEnergy());
                _FW.getHUD().setTotalLevelTime(_FW.getLevel().getTotalLevelTime());
                _FW.restartClock();
                _FW.getLevel().resetTimer();
                _FW.setDifficulty(_FW.getOptionsMenu().getDifficulty());

                _FW.getGOC().setGameMode(_FW.getGameMode());
                _FW.getGOC().setLevel(_FW.getLevel().getLevel());
            } else if (_FW.getGameState() == GameState::Highscores) {
                _FW.getHighscoreMenu().loadScoreTable();
            } else if (_FW.getGameState() == GameState::Options) {
                _FW.getOptionsMenu().enableDifficultySelection(true);
                _FW.getOptionsMenu().enableGameModeSelection(true);
                _FW.getOptionsMenu().setReturnState(GameState::Main);
            } else if (_FW.getGameState() == GameState::MultiplayerSelection) {
                _FW.getMultiplayerMenu().resetFeedback();
            }
            break;
        case GameState::Highscores:
            _FW.getGameState() = _FW.getHighscoreMenu().handleEvents(renderWindow);
            break;
        case GameState::Options:
            _FW.getGameState() = _FW.getOptionsMenu().handleEvents(renderWindow);
            if (_FW.getGameState() != GameState::Options) {
                _FW.getOptionsMenu().saveOptions(_FW.getMultiplayerMenu());
            }
            _FW.setVolume(_FW.getOptionsMenu().getVolume());
            _FW.setFPS(_FW.getOptionsMenu().getFPS());
            break;
        case GameState::About:
            _FW.setGameState(_FW.getAboutScreen().handleEvents(renderWindow));
            break;
        case GameState::Loading:
            while (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _FW.getGameState() = GameState::Exiting;
                }
            }
            break;
        case GameState::LevelUp:
            while (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _FW.getGameState() = GameState::Exiting;
                }
            }
            break;
        case GameState::BossFight:
            while (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _FW.getGameState() = GameState::Exiting;
                } else {
                    if (event.type == sf::Event::MouseLeft || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
                        sf::Joystick::isButtonPressed(0, 7)) {
                        _FW.getGameState() = GameState::Pause;
                        _FW.getPauseMenu().setReturnState(GameState::BossFight);
                    } else {
                        _FW.getGOC().handleEvent(event);
                    }
                }
            }
            break;
        case GameState::GameOver:
            _FW.setVolume(_FW.getOptionsMenu().getVolume());
            _FW.getGameState() = _FW.getGameOverScreen().handleEvents(renderWindow);
            if (_FW.getGameState() == GameState::Main) {
                _FW.resetGame();
            }
            break;
        case GameState::MultiplayerSelection:
            _FW.getGameState() = _FW.getMultiplayerMenu().handleEvents(renderWindow);
            if (_FW.getGameState() != GameState::MultiplayerSelection) {
                _FW.getOptionsMenu().saveOptions(_FW.getMultiplayerMenu());
            }
            if (_FW.getGameState() == GameState::Lobby) {
                _FW.getNetworkHandle().setState(NetworkState::Lobby);
                _FW.getNetworkHandle().setRelation(NetworkRelation::Host);

                GameObjectFactory::setDeltaID(0);

                _FW.getMultiplayerLobby().EnableButtons(true);
                _FW.getMultiplayerLobby().setAdminTable(true);
                _FW.getMultiplayerLobby().resetRdyButton();
                _FW.getMultiplayerLobby().resetTable();
                _FW.getMultiplayerLobby().addPlayer(_FW.getNetworkHandle().getMyName(), true);

                _FW.initializeNetworkThread();
            }

            break;
        case GameState::Connecting:
            _FW.getGameState() = _FW.getMultiplayerMenu().handleEvents(renderWindow);
            break;
        case GameState::Lobby:
            if (_FW.getMultiplayerMenu().getCreatedLobby() == 0 && _FW.getNetworkHandle().getState() == NetworkState::NoNetState) {
                _FW.getNetworkHandle().setState(NetworkState::Lobby);
                _FW.getNetworkHandle().setRelation(NetworkRelation::Client);

                GameObjectFactory::setDeltaID(
                        (sf::Uint64) std::pow(2.0f, 16.0f) * (sf::Uint64) std::pow(2.0f, 16.0f) / 2);

                _FW.getMultiplayerLobby().EnableButtons(false);
                _FW.getMultiplayerLobby().setAdminTable(false);
                _FW.getMultiplayerLobby().resetTable();
                _FW.getMultiplayerLobby().addPlayer(_FW.getNetworkHandle().getMemberName(), true);
                _FW.getMultiplayerLobby().addPlayer(_FW.getNetworkHandle().getMyName(), false);

                _FW.initializeNetworkThread();
            }

            _FW.getGameState() = _FW.getMultiplayerLobby().handleEvents(renderWindow);
            _FW.setCurrentCarSkinIndex(_FW.getMultiplayerLobby().getCarIndex());

            if (_FW.getCurrentCarSkinIndex() < 0) {
                _FW.setCurrentCarSkinIndex(_FW.getNumberOfCarsAvailable() - 1);
            } else if (_FW.getCurrentCarSkinIndex() >= _FW.getNumberOfCarsAvailable()) {
                _FW.setCurrentCarSkinIndex(0);
            }
            _FW.getMultiplayerLobby().setCarIndex(_FW.getCurrentCarSkinIndex());

            _FW.updateMPCarSelection();

            if (_FW.getGameState() == GameState::Countdown) {
                _FW.getHUD().setMaxHealth(_FW.getMPGOC().getPlayerCar().getMaxHealth());
                _FW.getHUD().setMaxEnergy(_FW.getMPGOC().getPlayerCar().getMaxEnergy());
                _FW.getHUD().setTotalLevelTime(_FW.getLevel().getTotalLevelTime());
                _FW.restartClock();
                _FW.getLevel().resetTimer();
                _FW.setDifficulty(_FW.getOptionsMenu().getDifficulty());
                _FW.getGameMode() = _FW.getOptionsMenu().getGameMode();
                _FW.getGOC().setGameMode(_FW.getGameMode());
                _FW.getMPGOC().setLevel(_FW.getLevel().getLevel());

                /*sf::Packet packet;
                _FW.getMPGOC().getPlayerCar() >> packet;
                _FW.getNetworkHandle().addPacket(NetworkCommunication::CreateGameObject, packet); */
            }
            break;
        case GameState::Countdown:
            _FW.setGameState(_FW.getCountdown().handleEvents(renderWindow));
            break;
        case GameState::RunningMultiplayer:
            if (renderWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    _FW.getGameState() = GameState::Exiting;
                } else if (event.type == sf::Event::MouseLeft) {
                    _FW.getPauseMultiplayerMenu().setReturnState(_FW.getGameState());
                    _FW.getGameState() = GameState::PauseMultiplayer;
                } else {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 7)) {
                        _FW.getPauseMultiplayerMenu().setReturnState(_FW.getGameState());
                        _FW.getGameState() = GameState::PauseMultiplayer;
                    } else {
                        _FW.getMPGOC().handleEvent(event);
                    }
                }
            }
            break;
        case GameState::PauseMultiplayer:
            _FW.getMPGOC().stopSounds();
            _FW.getGameState() = _FW.getPauseMultiplayerMenu().handleEvents(renderWindow);
            if (_FW.getGameState() == GameState::RunningMultiplayer) {
                _FW.restartClock();
            } else if (_FW.getGameState() == GameState::Main) {
                _FW.resetGame();
            } else if (_FW.getGameState() == GameState::Options) {
                _FW.getOptionsMenu().enableDifficultySelection(false);
                _FW.getOptionsMenu().enableGameModeSelection(false);
                _FW.getOptionsMenu().setReturnState(GameState::PauseMultiplayer);
            }

            break;
    }
}
