#include "stdafx.h"
#include "Framework/Framework.h"

Framework::Framework() : _TimeSinceLastUpdate(sf::Time::Zero), _FrameTime(0), _IsRunning(true), _LastFPSCheck(),
                         _LastFPSPrint(), _GameObjectManager(*this), _LevelManager(*this), _HighscoreManager(*this) {


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

    //Seed
    srand((unsigned int) time(NULL));

    _DisplayedGameScreens = initGameScreens();

    // FIXME Loading with a thread causes SIGSEGV
//    _LoadingThread = std::thread(&Framework::load, this);
//    _LoadingThread.detach();
    load();

    //Multiplayer
// FIXME multiplayer needs help
//    _MultiplayerMenu.setNetworkHandle(&_NetworkHandle);
//    _MultiplayerMenu.setPlayerName("Name");
//    _MultiplayerLobby.setNetworkHandle(&_NetworkHandle);
//    _MPGOCClient.setNetworkHandle(&_NetworkHandle, false);
    //_MPGOCServer.setNetworkHandle(&_NetworkHandle, true);
}

Framework::~Framework() {
//    _CarSkins.clear();
}

void Framework::run() {

    _FrameTime = 1 / _OptionsManager.getFPS();

    while (_IsRunning /*|| _NetworkHandle.getState() != NetworkState::NoNetState*/) {
        sf::Time elapsedTime = _Clock.restart();

        _TimeSinceLastUpdate += elapsedTime;

        while (_TimeSinceLastUpdate > sf::seconds(1 / _OptionsManager.getFPS())) {
            _TimeSinceLastUpdate -= sf::seconds(1 / _OptionsManager.getFPS());
            _FrameTime = 1 / _OptionsManager.getFPS();

            handleEvents();

            update(_FrameTime);
        }

        playSounds();
        render();

        // TODO Try 1:
        /*handleEvent();

        update();

        playSounds();

        render();

        sf::Time elapsedTime = _Clock.restart();

        if (elapsedTime < sf::seconds(1 / _OptionsManager.getFPS()))
        {
            _FrameTime = 1 / _OptionsManager.getFPS();
            sf::sleep(sf::seconds(1.0f / _OptionsManager.getFPS()) - elapsedTime);
        }
        else
        {
            _FrameTime = elapsedTime.asSeconds();
        } */



        // TODO Try 2:
        /*if (measureTime()) {

            render();
        }

        handleEvent();

        update();

        playSounds();


        sf::sleep(sf::seconds(1.0f / 32.f)); */
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

bool Framework::measureTime() {
    _FrameTime = _Clock.restart().asMicroseconds() / 1000000.0f;
    _LastFPSCheck += _FrameTime;
    _LastFPSPrint += _FrameTime;

    if (_LastFPSCheck >= 1 / _OptionsManager.getFPS()) {
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
        // FIXME reenable Music
//        if (!_MenuMusic.openFromFile("Resources/Sound/Music/menu1.ogg")) {
//            std::cout << "Couldn't load music" << std::endl;
//        }

        // FIXME ignoring this for now
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
    _Clock.restart();
    _GameObjectManager.resetGameObjects();
    _LevelManager.resetToLevelOne();
}

void Framework::stop() {
    _IsRunning = false;
//    _NetworkHandle.setRelation(NetworkRelation::NoRel);
//    _NetworkHandle.setState(NetworkState::NoNetState);
    _RenderWindow.close();
    _MenuMusic.stop();
    _LevelManager.stopMusic();
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

void Framework::restartClock() {
    _Clock.restart();
}

//void Framework::initializeNetworkThread() {
//    _NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
//    _NetworkThread.detach();
//}

void Framework::updateMPCarSelection() {
//    _MPGOCClient.getPlayerCar().setTexture((*_CarSkins.at((unsigned long) _OptionsManager.getCurrentCarSkinIndex())));
//    _MPGOCClient.getPlayerCar().setStats(_OptionsManager.getCurrentCarSkinIndex());
}

std::vector<GameScreen *> Framework::initGameScreens() {
    return GameScreenFactory::getInstance().getGameScreens(*this);
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
    return -1;
}

int Framework::getUPS() {
    return -1;
}
