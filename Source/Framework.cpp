#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() :
        _TimeSinceLastUpdate(sf::Time::Zero), _FrameTime(0), _FPS(60.0f), _IsRunning(true),
        _GameState(GameState::Loading), _GameMode(GameMode::Standard), _LastFPSCheck(), _LastFPSPrint(),
        _CurrentCarSkinIndex(), _Score(), _Renderer(*this), _Updater(*this), _EventHandler(*this) {

    //Seed
    srand((unsigned int) time(NULL));

    //Multiplayer
    _MultiplayerMenu.setNetworkHandle(&_NetworkHandle);
    _MultiplayerMenu.setPlayerName("Name");
    _MultiplayerLobby.setNetworkHandle(&_NetworkHandle);
    _MPGOCClient.setNetworkHandle(&_NetworkHandle, false);
    //_MPGOCServer.setNetworkHandle(&_NetworkHandle, true);
}

Framework::~Framework() {
    _CarSkins.clear();
}

void Framework::run() {
    _FrameTime = 1 / _FPS;

    while (_IsRunning || _NetworkHandle.getState() != NetworkState::NoNetState) {
        sf::Time elapsedTime = _Clock.restart();

        _TimeSinceLastUpdate += elapsedTime;

        while (_TimeSinceLastUpdate > sf::seconds(1 / _FPS)) {
            _TimeSinceLastUpdate -= sf::seconds(1 / _FPS);
            _FrameTime = 1 / _FPS;

            handleEvents();

            update();
        }

        playSounds();
        render();

        /*handleEvents();

        update();

        playSounds();

        render();

        sf::Time elapsedTime = _Clock.restart();

        if (elapsedTime < sf::seconds(1 / _FPS))
        {
            _FrameTime = 1 / _FPS;
            sf::sleep(sf::seconds(1.0f / _FPS) - elapsedTime);
        }
        else
        {
            _FrameTime = elapsedTime.asSeconds();
        } */




        /*if (measureTime()) {

            render();
        }

        handleEvents();

        update();

        playSounds();


        sf::sleep(sf::seconds(1.0f / 32.f)); */
    }
}

void Framework::render() {
    _Renderer.render();
}

void Framework::handleEvents() {
    _EventHandler.handleEvents(_Renderer.getRenderWindow());
}

void Framework::update() {
    _Updater.update(_FrameTime);
}

void Framework::playSounds() {
    if (_GameState == GameState::Running ||
        _GameState == GameState::BossFight ||
        _GameState == GameState::LevelUp ||
        _GameState == GameState::Pause ||
        (_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause)) {
        _MenuMusic.stop();
        _Level.playMusic();
        _GameObjectContainer.playSounds();
        if (_GameState == GameState::LevelUp) {
            _LevelUpScreen.playSound();
        }
    } else if (_GameState == GameState::Main || _GameState == GameState::Highscores ||
               (_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause)) {
        _Level.pauseMusic();
        if (_MenuMusic.getStatus() == sf::Sound::Stopped || _MenuMusic.getStatus() == sf::Sound::Paused) {
            _MenuMusic.play();
        }
    } else if (_GameState == GameState::GameOver) {
        _Level.stopMusic();
        _GameOverScreen.playSounds();
    } else if (_GameState == GameState::Exiting) {
        _Level.stopMusic();
        _MenuMusic.stop();
    }
}

bool Framework::measureTime() {
    _FrameTime = _Clock.restart().asMicroseconds() / 1000000.0f;
    _LastFPSCheck += _FrameTime;
    _LastFPSPrint += _FrameTime;

    if (_LastFPSCheck >= 1 / _FPS) {
        if (_LastFPSPrint > 1) {
            //std::cout << "FPS: " << 1 / _LastFPSCheck << std::endl;
            _LastFPSPrint = 0;
        }
        _LastFPSCheck = 0;
        return true;
    }
    return false;
}

void Framework::load() {
    try {
        if (!_MenuMusic.openFromFile("Resources/Sound/Music/menu1.ogg")) {
            std::cout << "Couldn't load music" << std::endl;
        }

        for (unsigned int i = 1; i < 7; i++) {
            sf::Texture texture;
            if (texture.loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) {
                texture.setSmooth(true);
                _CarSkins.push_back(std::make_shared<sf::Texture>(texture));
            }
        }

        _GameObjectContainer.load();
        _GameObjectContainer.setCarSkins(_CarSkins);
        _GameObjectContainer.resetGameObjects(0);


        _MPGOCClient.load();
        _MPGOCClient.setCarSkins(_CarSkins);
        _MPGOCClient.resetGameObjects(0);


        _GameOverScreen.load();

        _OptionsMenu.loadOptions(_MultiplayerMenu);
        _FPS = _OptionsMenu.getFPS();
        setVolume(_OptionsMenu.getVolume());

        _Level.load();
        _Level.resetLevel();
    }
    catch (...) {
        std::exit;
    }
}

void Framework::resetGame() {
    _Clock.restart();
    _Score = 0;
    _GameObjectContainer.resetGameObjects(_CurrentCarSkinIndex);
    _Level.resetLevel();
}

void Framework::stop() {
    _IsRunning = false;
    _NetworkHandle.setRelation(NetworkRelation::NoRel);
    _NetworkHandle.setState(NetworkState::NoNetState);
    _Renderer.closeRenderWindow();
    _MenuMusic.stop();
    _Level.stopMusic();
}

void Framework::setVolume(float volume) {
    sf::Listener::setGlobalVolume(volume * 7);
    _MenuMusic.setVolume(volume * 9);
    _Level.setVolume(volume * 7);
    _GameObjectContainer.setVolume((float) (volume * 2.7));
    _LevelUpScreen.setVolume(volume * 100);
    _GameOverScreen.setVolume(volume * 10);
}

void Framework::setDifficulty(int Difficulty) {
    _Level.setDifficulty(Difficulty);
    _GameObjectContainer.setDifficulty(Difficulty);
}

void Framework::addScore() {
    float Multiplier = _OptionsMenu.getMultiplierList()[(int) _GameMode];

    _Score += _GameObjectContainer.getCarScore() * Multiplier;

    switch (_OptionsMenu.getDifficulty()) {
        case 0:
            _Score += 5 * _Level.getLevel() * Multiplier * _FrameTime;
            break;
        case 1:
            _Score += 10 * (int) std::pow((float) _Level.getLevel(), 1.15f) * Multiplier * _FrameTime;
            break;
        case 2:
            _Score += 30 * (int) std::pow((float) _Level.getLevel(), 1.3f) * Multiplier * _FrameTime;
            break;
        case 3:
            _Score += 60 * (int) std::pow((float) _Level.getLevel(), 1.6f) * Multiplier * _FrameTime;
            break;
        default:
            break;
    }
}

void Framework::restartClock() {
    _Clock.restart();
}

void Framework::initializeNetworkThread() {
    _NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
    _NetworkThread.detach();
}

void Framework::updateMPCarSelection() {
    _MPGOCClient.getPlayerCar().setTexture((*_CarSkins.at((unsigned long) _CurrentCarSkinIndex)));
    _MPGOCClient.getPlayerCar().setStats(_CurrentCarSkinIndex);
}

void Framework::updateCarSelection() {
    _GameObjectContainer.getPlayerCar()->setTexture((*_CarSkins.at((unsigned long) _CurrentCarSkinIndex)));
    _GameObjectContainer.getPlayerCar()->setStats(_CurrentCarSkinIndex);
}
