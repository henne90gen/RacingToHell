#include "stdafx.h"
#include "Framework/Framework.h"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameObjectManager(*this), _LevelManager(*this),
                         _HighscoreManager(*this), _LoadingThread(&Framework::load, &*this) {

    _GameStates.push_back(GameState::Loading);

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

    // Seed for random number generators
    srand((unsigned int) time(NULL));

    reloadGameScreens();

    _LoadingThread.launch();


//    Multiplayer
//    FIXME multiplayer needs help
//    _MultiplayerMenu.setNetworkHandle(&_NetworkHandle);
//    _MultiplayerMenu.setPlayerName("Name");
//    _MultiplayerLobby.setNetworkHandle(&_NetworkHandle);
//    _MPGOCClient.setNetworkHandle(&_NetworkHandle, false);
//    _MPGOCServer.setNetworkHandle(&_NetworkHandle, true);
}

Framework::~Framework() {}

void Framework::run() {
//    Take a look at this for examples: http://gameprogrammingpatterns.com/game-loop.html
//    Update at a maximum of 1000 UPS
//    Render at modular FPS
//    All measurements are in microseconds

    while (_IsRunning) {
        sf::Clock renderClock;
        float microSecondsPerFrame = 1000000.0f / (_OptionsManager.getFPS() + _OptionsManager.getFPS() / 10.0f);
        _UpdateTime = microSecondsPerFrame / 1000000.0f;

        unsigned int updatesDone = 0;
        sf::Int64 totalUpdateTime = 0;
        while (totalUpdateTime < microSecondsPerFrame) {
            sf::Clock updateClock;

            handleEvents();
            update(_UpdateTime);
            updatesDone++;

            sf::Int64 updateTime = updateClock.getElapsedTime().asMicroseconds();
            float minTimePerUpdate = 1000000.0f / 1080.0f;
            if (updateTime <= minTimePerUpdate) {
                sf::sleep(sf::microseconds((int) (minTimePerUpdate - updateTime)));
            }
            updateTime = updateClock.restart().asMicroseconds();
            _UpdateTime = updateTime / 1000000.0f;
            totalUpdateTime += updateTime;
        }

        render();
        playSounds();

        sf::Int64 renderTime = renderClock.getElapsedTime().asMicroseconds();
        _FrameTime = renderTime / 1000000.0f;
    }
}

void Framework::render() {

    setMouseVisibility();

    for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->render(_RenderWindow);
    }

    _RenderWindow.display();
}

void Framework::handleEvents() {
    sf::Event event;
    while (_RenderWindow.pollEvent(event)) {
        for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
            _DisplayedGameScreens.at(i)->handleEvent(event);
        }
    }
}

void Framework::update(float frameTime) {

    if (getCurrentGameState() != GameState::Pause &&
        !(getCurrentGameState() == GameState::Options && getLastGameState() == GameState::Pause)) {
        _LevelManager.update(frameTime);
    }

    if (getCurrentGameState() == GameState::Running) {
        _GameObjectManager.update(frameTime);
    }

    for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->update(frameTime);
    }
}

void Framework::load() {
    try {
        // TODO enable Music again
//        if (!_MenuMusic.openFromFile("Resources/Sound/Music/menu1.ogg")) {
//            std::cout << "Couldn't load music" << std::endl;
//        }

//        FIXME ignoring this for now
//        _MPGOCClient.load();
//        _MPGOCClient.setCarSkins(_CarSkins);
//        _MPGOCClient.resetGameObjects(0);

        _OptionsManager.loadOptions();

        _LevelManager.load();
        _LevelManager.resetToLevelOne();

        _GameObjectManager.load();
        _GameObjectManager.resetGameObjects();

        advanceToGamState(GameState::LoadingToMain);
    }
    catch (...) {
        std::exit;
    }
}


void Framework::reset() {
    _GameObjectManager.resetGameObjects();
    _LevelManager.resetToLevelOne();
}

void Framework::stop() {
    _IsRunning = false;
    _RenderWindow.close();
    _MenuMusic.stop();
    _LevelManager.stopMusic();

//    _NetworkHandle.setRelation(NetworkRelation::NoRel);
//    _NetworkHandle.setState(NetworkState::NoNetState);
}

void Framework::playSounds() {
    // FIXME sound is broken
    /*
    if (_GameState == GameState::Running ||
        _GameState == GameState::BossFight ||
        _GameState == GameState::LevelUp ||
        _GameState == GameState::Pause ||
        (_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause)) {
        _MenuMusic.stop();
        _LevelManager.playMusic();
        _GameObjectContainer.playSounds();
        if (_GameState == GameState::LevelUp) {
            _LevelUpScreen.playSound();
        }
    } else if (_GameState == GameState::MainMenu || _GameState == GameState::Highscores ||
               (_GameState == GameState::Options && _OptionsMenu.getReturnState() == GameState::Pause)) {
        _LevelManager.pauseMusic();
        if (_MenuMusic.getStatus() == sf::Sound::Stopped || _MenuMusic.getStatus() == sf::Sound::Paused) {
            _MenuMusic.play();
        }
    } else if (_GameState == GameState::GameOver) {
        _LevelManager.stopMusic();
        _GameOverScreen.playSounds();
    } else if (_GameState == GameState::Exiting) {
        _LevelManager.stopMusic();
        _MenuMusic.stop();
    }
     */
}

void Framework::setVolume(float volume) {
    // FIXME sound is broken
    /*
    sf::Listener::setGlobalVolume(volume * 7);
    _MenuMusic.setVolume(volume * 9);
    _LevelManager.setVolume(volume * 7);
    _GameObjectContainer.setVolume((float) (volume * 2.7));
    _LevelUpScreen.setVolume(volume * 100);
    _GameOverScreen.setVolume(volume * 10);
    */
}

void Framework::reloadGameScreens() {
    _DisplayedGameScreens = GameScreenFactory::getInstance().getGameScreens(*this);
}

void Framework::setMouseVisibility() {
    bool visible = false;
    switch (getCurrentGameState()) {
        case GameState::PauseMultiplayer:
        case GameState::Lobby:
        case GameState::Connecting:
        case GameState::MultiplayerSelection:
        case GameState::GameOver:
        case GameState::About:
        case GameState::Options:
        case GameState::Highscores:
        case GameState::Pause:
        case GameState::MainMenu:
        case GameState::LoadingToMain:
            visible = true;
            break;
        case GameState::Running:
        case GameState::RunningMultiplayer:
        case GameState::Countdown:
        case GameState::Loading:
        case GameState::BossFight:
        case GameState::LevelUp:
            visible = false;
            break;
        case GameState::Exiting:
            break;
        case GameState::GameOverMultiplayer:
            break;
        case GameState::BossFightMultiplayer:
            break;
        case GameState::Empty:
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

bool Framework::isMouseVisible() {
    switch (getCurrentGameState()) {
        case GameState::PauseMultiplayer:
        case GameState::Lobby:
        case GameState::Connecting:
        case GameState::MultiplayerSelection:
        case GameState::GameOverMultiplayer:
        case GameState::GameOver:
        case GameState::About:
        case GameState::Options:
        case GameState::Highscores:
        case GameState::Pause:
        case GameState::MainMenu:
        case GameState::Loading:
        case GameState::LoadingToMain:
            return true;
        case GameState::Running:
        case GameState::RunningMultiplayer:
        case GameState::Countdown:
        case GameState::BossFight:
        case GameState::LevelUp:
        case GameState::Exiting:
        case GameState::BossFightMultiplayer:
            return false;
        default:
            return true;
    }
}

void Framework::advanceToGamState(GameState gameState) {
    if (_GameStates.at(_GameStates.size() - 1) != gameState) {
        _GameStates.push_back(gameState);
    }
    _DisplayedGameScreens = GameScreenFactory::getInstance().getGameScreens(*this);
}

GameState Framework::getCurrentGameState() {
    if (_GameStates.size() < 1) {
        return GameState::Empty;
    }
    return _GameStates.at(_GameStates.size() - 1);
}

GameState Framework::getLastGameState() {
    if (_GameStates.size() < 2) {
        return GameState::Empty;
    }
    return _GameStates.at(_GameStates.size() - 2);
}

void Framework::goBackGameState() {
    _GameStates.pop_back();
    advanceToGamState(_GameStates.at(_GameStates.size() - 1));
}

int Framework::getFPS() {
    return (int) (1 / _FrameTime);
}

int Framework::getUPS() {
    return (int) (1 / _UpdateTime);
}

//void Framework::initializeNetworkThread() {
//    _NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
//    _NetworkThread.detach();
//}

//void Framework::updateMPCarSelection() {
//    _MPGOCClient.getPlayerCar().setTexture((*_CarSkins.at((unsigned long) _OptionsManager.getCurrentCarSkinIndex())));
//    _MPGOCClient.getPlayerCar().setStats(_OptionsManager.getCurrentCarSkinIndex());
//}