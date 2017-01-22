//
// Created by henne on 27.11.16.
//

#pragma once

#include "Enums.h"
#include "Framework/LevelManager.h"
#include "UserInterface/Screen/GameScreen.h"
#include "UserInterface/Screen/EmptyScreen.h"
#include "UserInterface/Menu/MainMenu.h"
#include "UserInterface/Screen/LevelScreen.h"
#include "UserInterface/Screen/ExitScreen.h"
#include "UserInterface/Screen/GameObjectScreen.h"
#include "UserInterface/Menu/PauseMenu.h"
#include "UserInterface/Menu/HighscoreMenu.h"
#include "UserInterface/Menu/OptionsMenu.h"
#include "UserInterface/Menu/AboutMenu.h"
#include "UserInterface/Menu/GameOverMenu.h"
#include "UserInterface/Screen/LevelUpScreen.h"
#include "UserInterface/Screen/LoadingScreen.h"
#include "UserInterface/Screen/HUDScreen.h"
#include "UserInterface/Screen/DebugScreen.h"

class GameScreenFactory {
public:
    static GameScreenFactory &getInstance() {
        static GameScreenFactory instance;
        return instance;
    }

    GameScreenFactory(GameScreenFactory const &) = delete;

    void operator=(GameScreenFactory const &)  = delete;

    std::vector<GameScreen *> getGameScreens(Framework &framework);

private:
    GameScreenFactory() {}

    ~GameScreenFactory();

    // TODO maybe we can use a template here
    LoadingScreen *_LoadingScreen;

    LoadingScreen *getLoadingScreen(Framework &framework);

    MainMenu *_MainMenu;

    MainMenu *getMainMenu(Framework &framework);

    LevelScreen *_LevelScreen;

    LevelScreen *getLevelScreen(Framework &framework);

    OptionsMenu *_OptionsMenu;

    OptionsMenu *getOptionsMenu(Framework &framework);

    ExitScreen *_ExitScreen;

    ExitScreen *getExitScreen(Framework &framework);

    HighscoreMenu *_HighscoreMenu;

    HighscoreMenu *getHighscoreMenu(Framework &framework);

    AboutMenu *_AboutMenu;

    AboutMenu *getAboutMenu(Framework &framework);

    GameObjectScreen *_GameObjectScreen;

    GameObjectScreen *getGameObjectScreen(Framework &framework);

    HUDScreen *_HeadsUpDisplay;

    HUDScreen *getHeadsUpDisplayScreen(Framework &framework);

    PauseMenu *_PauseMenu;

    PauseMenu *getPauseMenu(Framework &framework);

    GameOverMenu *_GameOverMenu;

    GameOverMenu *getGameOverMenu(Framework &framework);

    LevelUpScreen *_LevelUpScreen;

    LevelUpScreen *getLevelUpScreen(Framework &framework);

    DebugScreen *_DebugScreen;

    DebugScreen *getDebugScreen(Framework &framework);
};
