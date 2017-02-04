//
// Created by henne on 27.11.16.
//

#pragma once

#include "Enums.h"
//#include "Framework/LevelManager.h"
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

class Framework;

class GameScreenFactory {
public:
    static GameScreenFactory &getInstance() {
        static GameScreenFactory instance;
        return instance;
    }

    GameScreenFactory(GameScreenFactory const &) = delete;

    void operator=(GameScreenFactory const &)  = delete;

    std::vector<std::shared_ptr<GameScreen>> getGameScreens(Framework &framework);

private:
    GameScreenFactory() {}

    ~GameScreenFactory() {}

    template<typename ScreenType>
    std::shared_ptr<ScreenType> getScreen(Framework &framework) {
        return std::make_shared<ScreenType>(framework);
    }
};
