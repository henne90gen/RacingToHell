//
// Created by henne on 27.11.16.
//

#include "stdafx.h"
#include "UserInterface/GameScreenFactory.h"
#include "Framework/Framework.h"

GameScreenFactory::~GameScreenFactory() {
    // TODO add all screens
    delete _LevelScreen;
    delete _LoadingScreen;
    delete _MainMenu;
    delete _OptionsMenu;
    delete _AboutMenu;
    delete _DebugScreen;
    delete _ExitScreen;
    delete _GameObjectScreen;
    delete _GameOverMenu;
    delete _HeadsUpDisplay;
    delete _HighscoreMenu;
    delete _LevelUpScreen;
    delete _PauseMenu;
}

std::vector<GameScreen *> GameScreenFactory::getGameScreens(Framework &framework) {
    std::vector<GameScreen *> screens = std::vector<GameScreen *>();

    screens.push_back(getLevelScreen(framework));

    switch (framework.getCurrentGameState()) {
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
            screens.push_back(getPauseMenu(framework));
            break;
        case GameState::Highscores:
            screens.push_back(getHighscoreMenu(framework));
            break;
        case GameState::Options:
            if (framework.getCurrentGameState() == GameState::Pause ||
                (framework.getLastGameState() == GameState::Pause &&
                 framework.getCurrentGameState() == GameState::Options)) {
                screens.push_back(getGameObjectScreen(framework));
                screens.push_back(getHeadsUpDisplayScreen(framework));
            }
            screens.push_back(getOptionsMenu(framework));
            break;
        case GameState::About:
            screens.push_back(getAboutMenu(framework));
            break;
        case GameState::BossFight:
            break;
        case GameState::LevelUp:
            screens.push_back(getLevelUpScreen(framework));
            break;
        case GameState::GameOver:
            screens.push_back(getGameObjectScreen(framework));
            screens.push_back(getGameOverMenu(framework));
            break;
        case GameState::Exiting:
            screens.push_back(getExitScreen(framework));
            break;
            // FIXME implement multiplayer stuff
        default:
            break;
    }

    if (framework.getOptionsManager().isDebugOn()) {
        screens.push_back(getDebugScreen(framework));
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

OptionsMenu *GameScreenFactory::getOptionsMenu(Framework &framework) {
    if (_OptionsMenu == nullptr) {
        _OptionsMenu = new OptionsMenu(framework);
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

PauseMenu *GameScreenFactory::getPauseMenu(Framework &framework) {
    if (_PauseMenu == nullptr) {
        _PauseMenu = new PauseMenu(framework);
    }
    return _PauseMenu;
}

GameOverMenu *GameScreenFactory::getGameOverMenu(Framework &framework) {
    if (_GameOverMenu == nullptr) {
        _GameOverMenu = new GameOverMenu(framework);
    }
    return _GameOverMenu;
}

LevelUpScreen *GameScreenFactory::getLevelUpScreen(Framework &framework) {
    if (_LevelUpScreen == nullptr) {
        _LevelUpScreen = new LevelUpScreen(framework);
    }
    return _LevelUpScreen;
}

DebugScreen *GameScreenFactory::getDebugScreen(Framework &framework) {
    if (_DebugScreen == nullptr) {
        _DebugScreen = new DebugScreen(framework);
    }
    return _DebugScreen;
}
