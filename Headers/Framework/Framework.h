#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObjectManager.h"
#include "UserInterface/Menu/MainMenu.h"
#include "UserInterface/Menu/PauseMenu.h"
#include "UserInterface/Menu/HighscoreMenu.h"
#include "UserInterface/Menu/OptionsMenu.h"
#include "UserInterface/Menu/AboutMenu.h"
#include "UserInterface/Menu/GameOverMenu.h"
#include "UserInterface/Menu/PauseMultiplayerMenu.h"
#include "UserInterface/Menu/MultiplayerMenu.h"
#include "UserInterface/Menu/MultiplayerLobby.h"
#include "UserInterface/Menu/Countdown.h"
#include "UserInterface/Menu/MenuItem/PlayerTable.h"
#include "UserInterface/Screen/LoadingScreen.h"
#include "UserInterface/Screen/LevelUpScreen.h"
#include "UserInterface/Screen/HUDScreen.h"
#include "UserInterface/GameScreenFactory.h"
#include "Multiplayer/NetworkHandle.h"
#include "Multiplayer/MPGameObjectContainer.h"
#include "LevelManager.h"
#include "OptionsManager.h"
#include "SoundManager.h"

/**
 * The Framework takes care of all the major aspects of the game.
 * Rendering, updating and event handling are all managed in here.
 */
class Framework {
public:
    Framework();

    ~Framework();

    /**
     * Starts the main game loop
     */
    void run();

    void stop();

    void load();

    void reset();

    void reloadGameScreens();

    void advanceToGamState(GameState gameState);

    void goBackGameState();

    GameState getCurrentGameState();

    GameState getLastGameState();

    LevelManager &getLevelManager() { return _LevelManager; }

    /**
     * The GameObjectManager (GOM) is the central entity for all GameObjects
     * @return Reference to the GameObjectManager
     */
    GameObjectManager &getGameObjectManager() { return _GameObjectManager; }

    /**
     * Shortcut for getGameObjectManager()
     * @return Reference to the GameObjectManager
     */
    GameObjectManager &getGOM() { return getGameObjectManager(); }

    /**
     *
     * @return Reference to the OptionsManager
     */
    OptionsManager &getOptionsManager() { return _OptionsManager; }

    /**
     *
     * @return Reference to the HighscoreManager
     */
    HighscoreManager &getHighscoreManager() { return _HighscoreManager; }

    /**
     *
     * @return Reference to the SoundManager
     */
    SoundManager &getSoundManager() { return _SoundManager; }

    bool isMouseVisible();

    /**
     * @return Current frames per second
     */
    int getFPS();

    /**
     * @return Current updates per second
     */
    int getUPS();

private:
    OptionsManager _OptionsManager;

    LevelManager _LevelManager;

    HighscoreManager _HighscoreManager;

    GameObjectManager _GameObjectManager;

    SoundManager _SoundManager;

    sf::RenderWindow _RenderWindow;

    std::vector<std::shared_ptr<GameScreen>> _DisplayedGameScreens;

    std::vector<GameState> _GameStates;

    float _FrameTime, _UpdateTime;

    bool _IsRunning;

    void render();

    void handleEvents();

    void update(float frameTime);

    /**
     * Takes care of hiding the mouse at the right moment.
     */
    void setMouseVisibility();
};
