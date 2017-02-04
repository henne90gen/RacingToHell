//
// Created by henne on 27.11.16.
//

#include "stdafx.h"
#include "UserInterface/GameScreenFactory.h"
#include "Framework/Framework.h"

std::vector<std::shared_ptr<GameScreen>> GameScreenFactory::getGameScreens(Framework &framework) {
    std::vector<std::shared_ptr<GameScreen>> screens;

    screens.push_back(getScreen<LevelScreen>(framework));

    switch (framework.getCurrentGameState()) {
        case GameState::Loading:
            screens.push_back(getScreen<LoadingScreen>(framework));
            break;
        case GameState::LoadingToMain:
            screens.push_back(getScreen<MainMenu>(framework));
            screens.push_back(getScreen<GameObjectScreen>(framework));
            screens.push_back(getScreen<LoadingScreen>(framework));
            break;
        case GameState::MainMenu:
            screens.push_back(getScreen<MainMenu>(framework));
            screens.push_back(getScreen<GameObjectScreen>(framework));
            break;
        case GameState::Running:
            screens.push_back(getScreen<GameObjectScreen>(framework));
            screens.push_back(getScreen<HUDScreen>(framework));
            break;
        case GameState::Pause:
            screens.push_back(getScreen<GameObjectScreen>(framework));
            screens.push_back(getScreen<HUDScreen>(framework));
            screens.push_back(getScreen<PauseMenu>(framework));
            break;
        case GameState::Highscores:
            screens.push_back(getScreen<HighscoreMenu>(framework));
            break;
        case GameState::Options:
            if (framework.getCurrentGameState() == GameState::Pause ||
                (framework.getLastGameState() == GameState::Pause &&
                 framework.getCurrentGameState() == GameState::Options)) {
                screens.push_back(getScreen<GameObjectScreen>(framework));
                screens.push_back(getScreen<HUDScreen>(framework));
            }
            screens.push_back(getScreen<OptionsMenu>(framework));
            break;
        case GameState::About:
            screens.push_back(getScreen<AboutMenu>(framework));
            break;
        case GameState::BossFight:
            break;
        case GameState::LevelUp:
            screens.push_back(getScreen<LevelUpScreen>(framework));
            break;
        case GameState::GameOver:
            screens.push_back(getScreen<GameObjectScreen>(framework));
            screens.push_back(getScreen<GameOverMenu>(framework));
            break;
        case GameState::Exiting:
            screens.push_back(getScreen<ExitScreen>(framework));
            return screens;
            // FIXME implement multiplayer stuff
        default:
            break;
    }

    if (framework.getOptionsManager().isDebugOn()) {
        screens.push_back(getScreen<DebugScreen>(framework));
    }

    return screens;
}
