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
#include "NetworkManager.h"

/**
 * The Framework takes care of all the major aspects of the game.
 * Rendering, updating and event handling are all managed in here.
 */
class Framework {
public:
    Framework();

    ~Framework() {}

    /**
     * Starts the main game loop
     */
    void run();

    /**
     * Stops the main game loop and closes the render window
     */
    void stop();

    /**
     * Loads resources into memory
     */
    void load();

    /**
     * Resets everything to be the way it was right after the game launched
     */
    void reset();

    /**
     * Drops all GameScreens and loads new GameScreens depending on the current GameState
     */
    void reloadGameScreens();

    /**
     * Moves forward to the given GameState
     * @param gameState Next GameState
     */
    void advanceToGameState(GameState gameState);

    /**
     * Goes back to the last GameState by removing the current one from the GameState's vector
     */
    void goBackGameState();

    /**
     * @return Current GameState
     */
    GameState getCurrentGameState();

    /**
     * Same as getLastGameState(1)
     */
    GameState getLastGameState();

    /**
     * Checks the GameState vector for the GameState that's a certain distance (back) away from the current GameState.
     * @param back Number of GameStates in the past
     * @return GameState from vector or GameState::Empty
     */
    GameState getLastGameState(int back);

    /**
     * The LevelManager takes care of the Level. (Keeping time, updating sprite, etc.)
     * @return Reference to the LevelManager
     */
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

    /**
     *
     * @return
     */
    bool isMouseVisible() { return _IsMouseVisible; }

    /**
     * @return Current frames per second
     */
    int getFPS();

    /**
     * @return Number of screens that are being displayed right now. (Be aware that the DebugScreen is included in this number)
     */
    int getNumScreens() { return (int) _DisplayedGameScreens.size(); }

    /**
     * Sets the vertical synchronisation property of the render window
     * @param vSync true = enabled, false = disabled
     */
    void setVSyncEnabled(bool vSync) { _RenderWindow.setVerticalSyncEnabled(vSync); }

    sf::Mutex &getMutex() { return _Mutex; }

private:
    OptionsManager _OptionsManager;

    LevelManager _LevelManager;

    HighscoreManager _HighscoreManager;

    GameObjectManager _GameObjectManager;

    SoundManager _SoundManager;

    NetworkManager _NetworkManager;

    sf::RenderWindow _RenderWindow;

    std::vector<std::shared_ptr<GameScreen>> _DisplayedGameScreens;

    std::vector<GameState> _GameStates;

    sf::Mutex _Mutex;

    float _FrameTime;

    bool _IsRunning, _IsMouseVisible;

    /**
     * Renders the current GameScreens
     */
    void render();

    /**
     * Processes any input that has occurred since the last call to this method
     */
    void handleEvents();

    /**
     * Updates the game
     * @param frameTime Amount of time that has passed since the last update
     */
    void update(float frameTime);

    /**
     * Takes care of hiding the mouse at the right moment.
     */
    void setMouseVisibility();
};
