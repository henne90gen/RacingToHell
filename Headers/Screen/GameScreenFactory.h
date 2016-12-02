//
// Created by henne on 27.11.16.
//

#pragma once

#include "GameState.h"
#include "GameScreen.h"
#include "Screen/EmptyScreen.h"
#include "Screen/Menu/MainMenu.h"
#include "LevelScreen.h"
#include "ExitScreen.h"
#include <Screen/Menu/PauseMenu.h>
#include <Screen/Menu/HighscoreMenu.h>
#include <Screen/Menu/OptionsMenu.h>
#include <Screen/Menu/AboutMenu.h>
#include <Screen/Menu/GameOverMenu.h>
#include <Screen/LevelUpScreen.h>
#include <Screen/LoadingScreen.h>
#include <Framework/LevelManager.h>

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

//    HeadsUpDisplay *_HeadsUpDisplay;


//    GameObjectScreen *_GameObjectScreen;

    OptionsMenu *_OptionsMenu;

    OptionsMenu *getOptionsMenu(Framework &framework);

    ExitScreen *_ExitScreen;

    ExitScreen *getExitScreen(Framework &framework);

    HighscoreMenu *_HighscoreMenu;

    HighscoreMenu *getHighscoreMenu(Framework &framework);

    AboutMenu *_AboutMenu;

    AboutMenu *getAboutMenu(Framework &framework);
};
