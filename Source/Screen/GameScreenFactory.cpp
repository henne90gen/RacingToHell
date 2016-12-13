//
// Created by henne on 27.11.16.
//

#include "stdafx.h"
#include "Screen/GameScreenFactory.h"
#include "Framework/Framework.h"

GameScreenFactory::~GameScreenFactory() {
    // TODO add all screens
    free(_LevelScreen);
    free(_LoadingScreen);
    free(_MainMenu);
    free(_OptionsMenu);
}

std::vector<GameScreen *> GameScreenFactory::getGameScreens(Framework &framework, GameState returnState) {
    std::vector<GameScreen *> screens = std::vector<GameScreen *>();

    screens.push_back(getLevelScreen(framework));

    switch (framework.getGameState()) {
        case GameState::Loading:
            screens.push_back(getLoadingScreen(framework));
            break;
        case GameState::LoadingToMain:
            screens.push_back(getMainMenu(framework));
            screens.push_back(getGameObjectScreen(framework));
            screens.push_back(getLoadingScreen(framework));
            break;
        case GameState::MainMenu:
            screens.push_back(getMainMenu(framework));
            screens.push_back(getGameObjectScreen(framework));
            break;
        case GameState::Running:
            screens.push_back(getGameObjectScreen(framework));
            screens.push_back(getHeadsUpDisplayScreen(framework));
            break;
        case GameState::Pause:
            screens.push_back(getGameObjectScreen(framework));
            screens.push_back(getHeadsUpDisplayScreen(framework));
            screens.push_back(getPauseMenu(framework, returnState));
            break;
        case GameState::Highscores:
            screens.push_back(getHighscoreMenu(framework));
            break;
        case GameState::Options:
            if (returnState == GameState::Pause) {
                screens.push_back(getGameObjectScreen(framework));
                screens.push_back(getHeadsUpDisplayScreen(framework));
            }
            screens.push_back(getOptionsMenu(framework, returnState));
            break;
        case GameState::About:
            screens.push_back(getAboutMenu(framework));
            break;
        case GameState::BossFight:
            break;
        case GameState::LevelUp:
//            screen = new LevelUpScreen(framework);
            break;
        case GameState::GameOver:
//            screen = new GameOverMenu(framework);
            break;
        case GameState::Exiting:
            screens.push_back(getExitScreen(framework));
            break;

            // FIXME implement multiplayer stuff
        case GameState::RunningMultiplayer:
            break;
        case GameState::PauseMultiplayer:
            break;
        case GameState::GameOverMultiplayer:
            break;
        case GameState::BossFightMultiplayer:
            break;
        case GameState::MultiplayerSelection:
            break;
        case GameState::Lobby:
            break;
        case GameState::Connecting:
            break;
        case GameState::Countdown:
            break;
    }
    return screens;
}

LoadingScreen *GameScreenFactory::getLoadingScreen(Framework &framework) {
    if (_LoadingScreen == nullptr) {
        _LoadingScreen = new LoadingScreen(framework);
    }
    return _LoadingScreen;
}

MainMenu *GameScreenFactory::getMainMenu(Framework &framework) {
    if (_MainMenu == nullptr) {
        _MainMenu = new MainMenu(framework);
    }
    return _MainMenu;
}

OptionsMenu *GameScreenFactory::getOptionsMenu(Framework &framework, GameState returnState) {
    if (_OptionsMenu == nullptr) {
        _OptionsMenu = new OptionsMenu(framework, returnState);
    }
    return _OptionsMenu;
}

LevelScreen *GameScreenFactory::getLevelScreen(Framework &framework) {
    if (_LevelScreen == nullptr) {
        _LevelScreen = new LevelScreen(framework);
    }
    return _LevelScreen;
}

ExitScreen *GameScreenFactory::getExitScreen(Framework &framework) {
    if (_ExitScreen == nullptr) {
        _ExitScreen = new ExitScreen(framework);
    }
    return _ExitScreen;
}

HighscoreMenu *GameScreenFactory::getHighscoreMenu(Framework &framework) {
    if (_HighscoreMenu == nullptr) {
        _HighscoreMenu = new HighscoreMenu(framework);
    }
    return _HighscoreMenu;
}

AboutMenu *GameScreenFactory::getAboutMenu(Framework &framework) {
    if (_AboutMenu == nullptr) {
        _AboutMenu = new AboutMenu(framework);
    }
    return _AboutMenu;
}

GameObjectScreen *GameScreenFactory::getGameObjectScreen(Framework &framework) {
    if (_GameObjectScreen == nullptr) {
        _GameObjectScreen = new GameObjectScreen(framework);
    }
    return _GameObjectScreen;
}

HUDScreen *GameScreenFactory::getHeadsUpDisplayScreen(Framework &framework) {
    if (_HeadsUpDisplay == nullptr) {
        _HeadsUpDisplay = new HUDScreen(framework);
    }
    return _HeadsUpDisplay;
}

PauseMenu *GameScreenFactory::getPauseMenu(Framework &framework, GameState returnState) {
    if (_PauseMenu == nullptr) {
        _PauseMenu = new PauseMenu(framework, returnState);
    }
    return _PauseMenu;
}
