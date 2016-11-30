#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "Screen/GameObjectScreen.h"
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
#include "HeadsUpDisplay.h"
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

    GameState &getGameState() {
        return _GameState;
    }

    LevelManager &getLevelManager() {
        return _LevelManager;
    }

    /**
     * The GameObjectContainer (GOC) is the central entity for all GameObjects
     * @return Reference to the GameObjectContainer
     */
//    GameObjectScreen &getGOC() {
//        return _GameObjectContainer;
//    }

    OptionsManager &getOptionsManager() {
        return _OptionsManager;
    }

    sf::RenderWindow &getRenderWindow() {
        return _RenderWindow;
    }

    void setGameState(GameState gameState);

    NetworkHandle &getNetworkHandle() {
        return _NetworkHandle;
    }

    /*MPGameObjectContainer &getMPGOC() {
        return _MPGOCClient;
    }*/

    void addScore();

    void setVolume(float Volume);

    void setDifficulty(int Difficulty);

    void restartClock();

    void load();

    void resetGame();

    void initializeNetworkThread();

    int getNumberOfCarsAvailable() {
        return (int) _CarSkins.size();
    }

    void updateCarSelection();

    void updateMPCarSelection();

private:
    OptionsManager _OptionsManager;

    LevelManager _LevelManager;

    sf::RenderWindow _RenderWindow;

    std::vector<GameScreen *> _DisplayedGameScreens;

    void setMouseVisibility();


    std::vector<GameScreen *> initGameScreens();

    sf::Clock _Clock;
    // Variables
    float _FrameTime, _LastFPSPrint, _LastFPSCheck;

    bool _IsRunning;

    sf::Time _TimeSinceLastUpdate;
    // GameObjects
    GameState _GameState;

//    GameObjectScreen _GameObjectContainer;
    // FIXME move to optionsmanager maybe?
    std::vector<std::shared_ptr<sf::Texture>> _CarSkins;

//    MPGameObjectContainer _MPGOCClient;
//    MPGameObjectContainer _MPGOCServer;

    // Music yo
    sf::Music _MenuMusic;

    //Multiplayer
    NetworkHandle _NetworkHandle;
    std::thread _NetworkThread;

    // Functions
    void render();

    void handleEvents();

    void update(float frameTime);

    void playSounds();

    bool measureTime();

};
