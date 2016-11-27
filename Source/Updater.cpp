//
// Created by henne on 27.11.16.
//

#include <GameState.h>
#include "Framework.hpp"

Updater::Updater(Framework &framework) : _FW(framework) {

}

void Updater::update(float frameTime) {
    switch (_FW.getGameState()) {
        case GameState::Running:
            if (_FW.getLevel().update(frameTime, _FW.getGameState())) {
                if (_FW.getGOC().emptyScreen()) {
                    _FW.getGOC().enterBossFight();
                    _FW.setGameState(GameState::BossFight);
                }
            }
            _FW.getGOC().update(frameTime, _FW.getLevel().getRoadSpeed());
            _FW.getHUD().update((int) _FW.getScore(), _FW.getGOC().getPlayerCar()->getHealth(),
                                (int) _FW.getGOC().getPlayerCar()->getEnergy(), _FW.getLevel().getLevel(),
                                _FW.getLevel().getLevelTime(), _FW.getGameMode());
            if (!_FW.getGOC().playerIsAlive()) {
                _FW.setGameState(GameState::GameOver);
            }

            _FW.addScore();
            break;
        case GameState::BossFight:
            _FW.getLevel().update(frameTime, _FW.getGameState());
            if (_FW.getGOC().bossIsDead()) {
                _FW.getLevelUpScreen().levelUp();
                _FW.setGameState(GameState::LevelUp);
            }
            _FW.getGOC().update(frameTime, _FW.getLevel().getRoadSpeed());
            _FW.getHUD().update((int) _FW.getScore(), _FW.getGOC().getPlayerCar()->getHealth(),
                                (int) _FW.getGOC().getPlayerCar()->getEnergy(), _FW.getLevel().getLevel(),
                                _FW.getLevel().getLevelTime(), _FW.getGameMode());
            if (!_FW.getGOC().playerIsAlive()) {
                _FW.setGameState(GameState::GameOver);
            }

            _FW.addScore();
            break;
        case GameState::LevelUp:
            if (_FW.getLevelUpScreen().update()) {
                _FW.restartClock();
                _FW.getGOC().getPlayerCar()->resetMovement();
                _FW.getGameState() = GameState::Running;
                _FW.getLevel().levelUp();
                _FW.setVolume(_FW.getOptionsMenu().getVolume());
                _FW.getGOC().setLevel(_FW.getLevel().getLevel());
                _FW.getHUD().setTotalLevelTime(_FW.getLevel().getTotalLevelTime());
            }
            break;
        case GameState::Main:
            _FW.getLevel().update(frameTime, _FW.getGameState());
            break;
        case GameState::GameOver:
            _FW.getGameOverScreen().update((int) _FW.getScore(), _FW.getLevel().getLevel());
            break;
        case GameState::Highscores:
            _FW.getLevel().update(frameTime, _FW.getGameState());
            break;
        case GameState::Options:
            _FW.getOptionsMenu().update(frameTime);
            if (_FW.getOptionsMenu().getReturnState() == GameState::Main) {
                _FW.getLevel().update(frameTime, _FW.getGameState());
            }
            break;
        case GameState::About:
            if (_FW.getOptionsMenu().getReturnState() == GameState::Main) {
                _FW.getLevel().update(frameTime, _FW.getGameState());
            }
            break;
        case GameState::Loading:
            if (!_FW.getLoadingScreen().isFadingAway()) {
                _FW.load();
                _FW.getOptionsMenu().setFPS(_FW.getFPS());
                _FW.getLoadingScreen().fadeAway();
            } else if (_FW.getLoadingScreen().isDoneFading()) {
                _FW.setGameState(GameState::Main);
            } else if (_FW.getLoadingScreen().isFadingAway()) {
                _FW.getLevel().update(frameTime, _FW.getGameState());
            }
            break;
        case GameState::Exiting:
            _FW.stop();
            break;
        case GameState::MultiplayerSelection:
            _FW.getLevel().update(frameTime, _FW.getGameState());
            break;
        case GameState::Connecting: {
            _FW.getLevel().update(frameTime, _FW.getGameState());
            NetworkCommunication netComm = _FW.getMultiplayerMenu().update(frameTime);
            if (netComm == NetworkCommunication::ConnectionSuccesfull) {
                _FW.getGameState() = GameState::Lobby;
            } else if (netComm == NetworkCommunication::ConnectionFailed ||
                       netComm == NetworkCommunication::WrongPassword) {
                _FW.getGameState() = GameState::MultiplayerSelection;
            }
            break;
        }
        case GameState::Lobby: {
            std::pair<NetworkCommunication, int> lastResponse = _FW.getNetworkHandle().getLastResponse();

            if (lastResponse.first == NetworkCommunication::Kick) {
                _FW.getGameState() = GameState::MultiplayerSelection;
                _FW.getMultiplayerMenu().resetFeedback();
                _FW.getMultiplayerMenu().setKickMessage();
                //_FW.getMultiplayerMenu().resetTextbox();
            } else if (lastResponse.first == NetworkCommunication::Disconnect && lastResponse.second == 0) {
                _FW.getGameState() = GameState::MultiplayerSelection;
                _FW.getMultiplayerMenu().resetFeedback();
                _FW.getMultiplayerMenu().setLobbyClosedMessage();
            } else if (lastResponse.first == NetworkCommunication::Disconnect && lastResponse.second == 1) {
                _FW.getMultiplayerLobby().removePlayer(1);
            } else if (lastResponse.first == NetworkCommunication::ConnectionSuccesfull &&
                       _FW.getNetworkHandle().getRelation() == NetworkRelation::Host) {
                _FW.getMultiplayerLobby().addPlayer(_FW.getNetworkHandle().getMemberName(), false);
            } else if (lastResponse.first == NetworkCommunication::Ready &&
                       _FW.getNetworkHandle().getRelation() == NetworkRelation::Host) {
                _FW.getMultiplayerLobby().setClientReady((bool) (lastResponse.second));
            } else if (lastResponse.first == NetworkCommunication::StartGame &&
                       _FW.getNetworkHandle().getRelation() == NetworkRelation::Client) {
                
                _FW.getCountdown().fastForward((float) lastResponse.second / (float) _FW.getNetworkHandle().getTickRate());
                _FW.getHUD().setMaxHealth(_FW.getMPGOC().getPlayerCar().getMaxHealth());
                _FW.getHUD().setMaxEnergy(_FW.getMPGOC().getPlayerCar().getMaxEnergy());
                _FW.getHUD().setTotalLevelTime(_FW.getLevel().getTotalLevelTime());
                _FW.restartClock();
                _FW.getLevel().resetTimer();
                _FW.setDifficulty(_FW.getOptionsMenu().getDifficulty());
                _FW.setGameMode(_FW.getOptionsMenu().getGameMode());
                _FW.getGOC().setGameMode(_FW.getGameMode());
                _FW.getMPGOC().setLevel(_FW.getLevel().getLevel());
                _FW.setGameState(GameState::Countdown);
            }

            /*if (lastResponse.first == NetworkCommunication::StartGame) {
                sf::Packet packet;
                _FW.getMPGOC().getPlayerCar() >> packet;
                _FW.getNetworkHandle().addPacket(NetworkCommunication::CreateGameObject, packet);
            }*/

            _FW.getNetworkHandle().setReceivedPackets(std::vector<sf::Packet>());

            _FW.getLevel().update(frameTime, _FW.getGameState());
            break;
        }
        case GameState::Countdown:
            if (_FW.getCountdown().update(frameTime)) {
                _FW.setGameState(GameState::RunningMultiplayer);
            }
            break;
        case GameState::RunningMultiplayer:
        case GameState::PauseMultiplayer:
            if (_FW.getLevel().update(frameTime, _FW.getGameState())) {
                if (_FW.getMPGOC().emptyScreen()) {
                    _FW.getMPGOC().enterBossFight();
                    _FW.setGameState(GameState::BossFightMultiplayer);
                }
            }

            _FW.getMPGOC().update(frameTime, _FW.getLevel().getRoadSpeed());
            _FW.getHUD().update(_FW.getScore(), _FW.getMPGOC().getPlayerCar().getHealth(),
                                _FW.getMPGOC().getPlayerCar().getEnergy(), _FW.getLevel().getLevel(),
                                _FW.getLevel().getLevelTime(),
                                _FW.getGameMode());
            if (!_FW.getMPGOC().playerIsAlive()) {
                _FW.setGameState(GameState::GameOverMultiplayer);
            }
            _FW.addScore();
            break;
    }
}
