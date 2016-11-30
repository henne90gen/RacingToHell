#include "stdafx.h"
#include "Framework/Framework.h"

Framework::Framework() :
        _TimeSinceLastUpdate(sf::Time::Zero), _FrameTime(0), _IsRunning(true),
        _GameState(GameState::Loading), _LastFPSCheck(), _LastFPSPrint() {

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
    _CarSkins.clear();
}

void Framework::run() {

    _FrameTime = 1 / _OptionsManager.getFPS();

    while (_IsRunning || _NetworkHandle.getState() != NetworkState::NoNetState) {
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

    for (int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->render(_RenderWindow);
    }

    _RenderWindow.display();
}

void Framework::handleEvents() {
    sf::Event event;
    while (_RenderWindow.pollEvent(event)) {
        for (int i = 0; i < _DisplayedGameScreens.size(); i++) {
            _DisplayedGameScreens.at(i)->handleEvent(event);
        }
    }
}

void Framework::update(float frameTime) {

    _LevelManager.update(frameTime, _GameState);

    for (int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->update(frameTime);
    }

    if (_GameState == GameState::Exiting) {
        exit(0);
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
    } else if (_GameState == GameState::Main || _GameState == GameState::Highscores ||
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

//        for (unsigned int i = 1; i < 7; i++) {
//            sf::Texture texture;
//            if (texture.loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) {
//                texture.setSmooth(true);
//                _CarSkins.push_back(std::make_shared<sf::Texture>(texture));
//            }
//        }

        // FIXME what should we do with this? updater maybe?
//        _GameObjectContainer.load();
//        _GameObjectContainer.setCarSkins(_CarSkins);
//        _GameObjectContainer.resetGameObjects(0);


        // FIXME ignoring this for now
//        _MPGOCClient.load();
//        _MPGOCClient.setCarSkins(_CarSkins);
//        _MPGOCClient.resetGameObjects(0);


        // FIXME write game over helper
//        _GameOverScreen.load();

//        _OptionsMenu.loadOptions(_MultiplayerMenu);
//        _OptionsManager.getFPS() = _OptionsMenu.getFPS();
//        setVolume(_OptionsMenu.getVolume());
        _OptionsManager.loadOptions();

        // FIXME reenable or move this
//        _LevelManager.load();
//        _LevelManager.resetToLevelOne();

        setGameState(GameState::LoadingToMain);
    }
    catch (...) {
        std::exit;
    }
}

void Framework::resetGame() {
    _Clock.restart();
    _OptionsManager.setScore(0);
//    _GameObjectContainer.resetGameObjects(_OptionsManager.getCurrentCarSkinIndex());
    _LevelManager.resetToLevelOne();
}

void Framework::stop() {
    _IsRunning = false;
    _NetworkHandle.setRelation(NetworkRelation::NoRel);
    _NetworkHandle.setState(NetworkState::NoNetState);
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

void Framework::setDifficulty(int Difficulty) {
    // FIXME move this into optionsmanager and decide where to move level
    _LevelManager.setDifficulty(Difficulty);
//    _GameObjectContainer.setDifficulty(Difficulty);
}

void Framework::addScore() {
    // FIXME write optionshelper
    /*
    float Multiplier = _OptionsMenu.getMultiplierList()[(int) _GameMode];

    _Score += _GameObjectContainer.getCarScore() * Multiplier;

    switch (_OptionsMenu.getDifficulty()) {
        case 0:
            _Score += 5 * _LevelManager.getLevel() * Multiplier * _FrameTime;
            break;
        case 1:
            _Score += 10 * (int) std::pow((float) _LevelManager.getLevel(), 1.15f) * Multiplier * _FrameTime;
            break;
        case 2:
            _Score += 30 * (int) std::pow((float) _LevelManager.getLevel(), 1.3f) * Multiplier * _FrameTime;
            break;
        case 3:
            _Score += 60 * (int) std::pow((float) _LevelManager.getLevel(), 1.6f) * Multiplier * _FrameTime;
            break;
        default:
            break;
    }
     */
}

void Framework::restartClock() {
    _Clock.restart();
}

void Framework::initializeNetworkThread() {
    _NetworkThread = std::thread(&NetworkHandle::run, &_NetworkHandle);
    _NetworkThread.detach();
}

void Framework::updateMPCarSelection() {
//    _MPGOCClient.getPlayerCar().setTexture((*_CarSkins.at((unsigned long) _OptionsManager.getCurrentCarSkinIndex())));
//    _MPGOCClient.getPlayerCar().setStats(_OptionsManager.getCurrentCarSkinIndex());
}

// FIXME reenable goc
void Framework::updateCarSelection() {
//    _GameObjectContainer.getPlayerCar()->setTexture((*_CarSkins.at((unsigned long) _OptionsManager.getCurrentCarSkinIndex())));
//    _GameObjectContainer.getPlayerCar()->setStats(_OptionsManager.getCurrentCarSkinIndex());
}

void Framework::setGameState(GameState gameState) {
    if (_GameState != gameState) {
        std::cout << "Changed gamestate " << (int) gameState << std::endl;
        _GameState = gameState;
        _DisplayedGameScreens = GameScreenFactory::getInstance().getGameScreens(*this);
    }
}

std::vector<GameScreen *> Framework::initGameScreens() {
    return GameScreenFactory::getInstance().getGameScreens(*this);
}

void Framework::setMouseVisibility() {
    bool visible;
    switch (getGameState()) {
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
        case GameState::LoadingToMain:
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
