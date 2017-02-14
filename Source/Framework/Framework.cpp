#include "stdafx.h"
#include "Framework/Framework.h"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _OptionsManager(*this), _GameObjectManager(*this),
                         _LevelManager(*this), _HighscoreManager(*this), _SoundManager(*this) {

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

    load();
}

void Framework::run() {
//    Take a look at this for examples: http://gameprogrammingpatterns.com/game-loop.html
//    Update at a maximum of 1000 UPS
//    Render at modular FPS
//    All measurements are in microseconds

	int sleepCounter = 0;

	while (_IsRunning) {
        sf::Clock renderClock;
        sf::Time targetFrameTime = sf::seconds(1.0f / _OptionsManager.getFPS());
        _FrameTime = targetFrameTime.asSeconds();

        handleEvents();
        update(_FrameTime);
        render();

        sf::Time actualFrameTime = renderClock.getElapsedTime();

        if (actualFrameTime >= targetFrameTime) {
            float delta = (actualFrameTime - targetFrameTime).asSeconds();
            std::cout << "Delta " << sleepCounter << ": " << delta << std::endl;
            update(delta);
            sleepCounter = 0;
        } else {
            sleepCounter++;
            sf::sleep(targetFrameTime - actualFrameTime);
        }

        _RenderWindow.display();
    }
}

void Framework::render() {
    setMouseVisibility();

    for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->render(_RenderWindow);
    }
}

void Framework::handleEvents() {
    sf::Event event;
    while (_RenderWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            advanceToGameState(GameState::Exiting);
            return;
        }
        for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
            _DisplayedGameScreens.at(i)->handleEvent(event);
        }
    }
}

void Framework::update(float frameTime) {
	//sf::Clock myClock;

    if (getCurrentGameState() != GameState::Pause &&
        !(getCurrentGameState() == GameState::Options && getLastGameState() == GameState::Pause)) {
        _LevelManager.update(frameTime);
    }


	//std::cout << "level " << myClock.restart().asSeconds() << std::endl;

    if (getCurrentGameState() == GameState::Running) {
        _GameObjectManager.update(frameTime);
    }


	//std::cout << "gom " << myClock.restart().asSeconds() << std::endl;

    _SoundManager.update();


	//std::cout << "sm " << myClock.restart().asSeconds() << std::endl;

    for (unsigned int i = 0; i < _DisplayedGameScreens.size(); i++) {
        _DisplayedGameScreens.at(i)->update(frameTime);
    }

	//std::cout << "screens " << myClock.restart().asSeconds() << std::endl;
}

void Framework::load() {
    // Display loading screen before starting to load
    LoadingScreen screen(*this);
    screen.render(_RenderWindow);
    _RenderWindow.display();

    try {
        std::cout << "Loading..." << std::endl;

        _OptionsManager.load();

        _SoundManager.load();

        _LevelManager.load();
        _LevelManager.resetToLevelOne();

        _GameObjectManager.load();
        _GameObjectManager.resetGameObjects();

        std::cout << "Done loading." << std::endl;

        advanceToGameState(GameState::LoadingToMain);
    } catch (...) { std::exit; }
}


void Framework::reset() {
    _GameObjectManager.resetGameObjects();
    _LevelManager.resetToLevelOne();
}

void Framework::stop() {
    _IsRunning = false;
    _RenderWindow.close();
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
        case GameState::GameOverMultiplayer:
            visible = true;
            break;
        case GameState::Running:
        case GameState::RunningMultiplayer:
        case GameState::Countdown:
        case GameState::Loading:
        case GameState::LevelUp:
        case GameState::Exiting:
        case GameState::BossFightMultiplayer:
        case GameState::Empty:
            visible = false;
            break;
    }

#ifdef SFML_SYSTEM_WINDOWS
    if (!visible) {
        int cursor = ShowCursor(0);
        while (cursor > 0) {
            cursor = ShowCursor(0);
        }
    } else {
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
        case GameState::LevelUp:
        case GameState::Exiting:
        case GameState::BossFightMultiplayer:
            return false;
        default:
            return true;
    }
}

void Framework::advanceToGameState(GameState gameState) {
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
    advanceToGameState(_GameStates.at(_GameStates.size() - 1));
}

int Framework::getFPS() {
    return (int) (1 / _FrameTime);
}
