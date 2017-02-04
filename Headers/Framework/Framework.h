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

#include <sstream>
#include <fstream>
// #include <thread>

class Framework {
public:
    /**
        The Framework takes care of all the major aspects of the game.
        Rendering, updating and event handling are all managed in here.
    */
    Framework();

    ~Framework();

    /**
        Starts the main game loop
    */
    void run();

    void stop();

    GameState getCurrentGameState();

    GameState getLastGameState();

    void advanceToGamState(GameState gameState);

    void goBackGameState();

    int getFPS();

    int getUPS();

    LevelManager &getLevelManager() {
        return _LevelManager;
    }

    /**
     * The GameObjectManager (GOM) is the central entity for all GameObjects
     * @return Reference to the GameObjectManager
     */
    GameObjectManager &getGameObjectManager() {
        return _GameObjectManager;
    }

    /**
     * Shortcut for getGameObjectManager()
     * @return
     */
    GameObjectManager &getGOM() {
        return getGameObjectManager();
    }

    OptionsManager &getOptionsManager() {
        return _OptionsManager;
    }

    HighscoreManager &getHighscoreManager() {
        return _HighscoreManager;
    }

//    NetworkHandle &getNetworkHandle() {
//        return _NetworkHandle;
//    }

    /*MPGameObjectContainer &getMPGOC() {
        return _MPGOCClient;
    }*/

    void setVolume(float Volume);

    void restartClock();

    void load();

    void reset();

//    void initializeNetworkThread();
//    void updateMPCarSelection();

    bool isMouseVisible();

    void reloadGameScreens();

private:
    OptionsManager _OptionsManager;

    LevelManager _LevelManager;

    HighscoreManager _HighscoreManager;

    GameObjectManager _GameObjectManager;


    sf::RenderWindow _RenderWindow;

    std::vector<std::shared_ptr<GameScreen>> _DisplayedGameScreens;

    std::vector<GameState> _GameStates;

    sf::Clock _Clock;

    sf::Time _TimeSinceLastUpdate;

    // Variables
    float _FrameTime, _LastFPSPrint, _LastFPSCheck;

    bool _IsRunning;

//    MPGameObjectContainer _MPGOCServer;

    // Music yo
    sf::Music _MenuMusic;

//    std::thread _NetworkThread;

    // Functions
    void render();

    void handleEvents();

    void update(float frameTime);

    void playSounds();

    void setMouseVisibility();

    bool measureTime();

//    MPGameObjectContainer _MPGOCClient;
    //Multiplayer
//    NetworkHandle _NetworkHandle;
};
