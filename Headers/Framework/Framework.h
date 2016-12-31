#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObjectManager.h"
#include "Screen/Menu/MainMenu.h"
#include "Screen/Menu/PauseMenu.h"
#include "Screen/Menu/HighscoreMenu.h"
#include "Screen/Menu/OptionsMenu.h"
#include "Screen/Menu/AboutMenu.h"
#include "Screen/Menu/GameOverMenu.h"
#include "Screen/LoadingScreen.h"
#include "Screen/LevelUpScreen.h"
#include "Screen/Menu/PauseMultiplayerMenu.h"
#include "Screen/Menu/MultiplayerMenu.h"
#include "Screen/Menu/MultiplayerLobby.h"
#include "Screen/Menu/Countdown.h"
#include "Screen/HUDScreen.h"
#include "Multiplayer/NetworkHandle.h"
#include "Multiplayer/MPGameObjectContainer.h"
#include "LevelManager.h"
#include "Screen/Menu/MenuItem/PlayerTable.h"
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

    GameState getCurrentGameState() {
        if (_GameStates.size() < 1) {
            return GameState::Empty;
        }
        return _GameStates.at(_GameStates.size() - 1);
    }

    GameState getLastGameState() {
        if (_GameStates.size() < 2) {
            return GameState::Empty;
        }
        return _GameStates.at(_GameStates.size() - 2);
    }

    void goBackOneGameState();

    LevelManager &getLevelManager() {
        return _LevelManager;
    }

    /**
     * The GameObjectContainer (GOC) is the central entity for all GameObjects
     * @return Reference to the GameObjectContainer
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

    sf::RenderWindow &getRenderWindow() {
        return _RenderWindow;
    }

    void setGameState(GameState gameState);

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

    void initializeNetworkThread();

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
