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
#include "Multiplayer/NetworkHandle.h"
#include "Multiplayer/MPGameObjectContainer.h"
#include "LevelManager.h"
#include "Renderer.h"
#include "Updater.h"
#include "EventHandler.h"
#include "OptionsManager.h"

#include <sstream>
#include <fstream>
#include <thread>

/**
 * Forward declarations
 */
class Renderer;

class Updater;

class EventHandler;

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

    void setGameState(GameState gameState);

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

//    NetworkHandle &getNetworkHandle() {
//        return _NetworkHandle;
//    }

    /*MPGameObjectContainer &getMPGOC() {
        return _MPGOCClient;
    }*/

    void addScore();

    void setVolume(float Volume);

    void restartClock();

    void load();

    void reset();

//    void initializeNetworkThread();

//    int getNumberOfCarsAvailable() {
//        return (int) _CarSkins.size();
//    }

    void updateMPCarSelection();

    bool isMouseVisible();

private:
    OptionsManager _OptionsManager;

    LevelManager _LevelManager;

    GameObjectManager _GameObjectManager;

    sf::RenderWindow _RenderWindow;

    std::vector<GameScreen *> _DisplayedGameScreens;

    std::vector<GameState> _GameStates;

    std::vector<GameScreen *> initGameScreens();

    std::thread _LoadingThread;
    sf::Clock _Clock;

    sf::Time _TimeSinceLastUpdate;

    // Variables
    float _FrameTime, _LastFPSPrint, _LastFPSCheck;

    bool _IsRunning;

//    MPGameObjectContainer _MPGOCClient;
//    MPGameObjectContainer _MPGOCServer;

    // Music yo
    sf::Music _MenuMusic;

    //Multiplayer
//    NetworkHandle _NetworkHandle;
//    std::thread _NetworkThread;

    // Functions
    void render();

    void handleEvents();

    void update(float frameTime);

    void playSounds();

    void setMouseVisibility();

    bool measureTime();

};
