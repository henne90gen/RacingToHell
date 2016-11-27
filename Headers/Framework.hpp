#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/GameObjectContainer.h"
#include "Menu/MainMenu.h"
#include "Menu/PauseMenu.h"
#include "Menu/HighscoreMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/AboutScreen.h"
#include "Menu/GameOverScreen.h"
#include "Menu/LoadingScreen.h"
#include "Menu/LevelUpScreen.h"
#include "Menu/PauseMultiplayerMenu.h"
#include "Menu/MultiplayerMenu.h"
#include "Menu/MultiplayerLobby.h"
#include "Menu/Countdown.h"
#include "HeadsUpDisplay.h"
#include "Multiplayer/NetworkHandle.h"
#include "Multiplayer/MPGameObjectContainer.h"
#include "Level/Level.h" 
#include "Menu/MenuItem/PlayerTable.h"
#include "Renderer.h"
#include "Updater.h"
#include "EventHandler.h"

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

    Level &getLevel() {
        return _Level;
    }

    LoadingScreen &getLoadingScreen() {
        return _LoadingScreen;
    }

    /**
     * The GameObjectContainer (GOC) is the central entity for all GameObjects
     * @return Reference to the GameObjectContainer
     */
    GameObjectContainer &getGOC() {
        return _GameObjectContainer;
    }

    HeadsUpDisplay &getHUD() {
        return _HeadsUpDisplay;
    }

    MainMenu &getMainMenu() {
        return _MainMenu;
    }

    PauseMenu &getPauseMenu() {
        return _PauseMenu;
    }

    HighscoreMenu &getHighscoreMenu() {
        return _HighscoreMenu;
    }

    OptionsMenu &getOptionsMenu() {
        return _OptionsMenu;
    }

    AboutScreen &getAboutScreen() {
        return _AboutScreen;
    }

    LevelUpScreen &getLevelUpScreen() {
        return _LevelUpScreen;
    }

    GameOverScreen &getGameOverScreen() {
        return _GameOverScreen;
    }

    MultiplayerMenu &getMultiplayerMenu() {
        return _MultiplayerMenu;
    }

    MultiplayerLobby &getMultiplayerLobby() {
        return _MultiplayerLobby;
    }

    PauseMultiplayerMenu &getPauseMultiplayerMenu() {
        return _PauseMultiplayerMenu;
    }

    Countdown &getCountdown() {
        return _Countdown;
    }

    void setGameState(GameState gameState) {
        _GameState = gameState;
    }

    GameMode &getGameMode() {
        return _GameMode;
    }

    void setGameMode(GameMode gameMode) {
        _GameMode = gameMode;
    }

    float getScore() {
        return _Score;
    }

    NetworkHandle &getNetworkHandle() {
        return _NetworkHandle;
    }

    MPGameObjectContainer &getMPGOC() {
        return _MPGOCClient;
    }

    int getCurrentCarSkinIndex() {
        return _CurrentCarSkinIndex;
    }

    void setCurrentCarSkinIndex(int newIndex) {
        _CurrentCarSkinIndex = newIndex;
    }

    void addScore();

    void setVolume(float Volume);

    void setDifficulty(int Difficulty);

    void restartClock();

    void load();

    float getFPS() {
        return _FPS;
    }

    void setFPS(float fps) {
        _FPS = fps;
    }

    void resetGame();

    void initializeNetworkThread();

    int getNumberOfCarsAvailable() {
        return (int) _CarSkins.size();
    }

    void updateCarSelection();

    void updateMPCarSelection();

private:
    // SFML-Graphics
//    sf::RenderWindow _RenderWindow;
    sf::Event _Event;
    sf::Clock _Clock;

    // Variables
    GameMode _GameMode;
    float _FrameTime, _LastFPSPrint, _LastFPSCheck, _Score, _FPS;
    bool _IsRunning;
    int _CurrentCarSkinIndex;
    sf::Time _TimeSinceLastUpdate;

    Renderer _Renderer;
    Updater _Updater;
    EventHandler _EventHandler;

    // GameObjects
    GameState _GameState;
    GameObjectContainer _GameObjectContainer;
    std::vector<std::shared_ptr<sf::Texture>> _CarSkins;

    //MPGameObjectContainer _MPGOCServer;
    MPGameObjectContainer _MPGOCClient;

    // Level
    Level _Level;

    // Music yo
    sf::Music _MenuMusic;

    // Menus and Screens
    MainMenu _MainMenu;
    PauseMenu _PauseMenu;
    HighscoreMenu _HighscoreMenu;
    OptionsMenu _OptionsMenu;
    AboutScreen _AboutScreen;
    GameOverScreen _GameOverScreen;
    LoadingScreen _LoadingScreen;
    LevelUpScreen _LevelUpScreen;
    HeadsUpDisplay _HeadsUpDisplay;
    MultiplayerMenu _MultiplayerMenu;
    MultiplayerLobby _MultiplayerLobby;
    PauseMultiplayerMenu _PauseMultiplayerMenu;
    Countdown _Countdown;

    //Multiplayer
    NetworkHandle _NetworkHandle;
    std::thread _NetworkThread;

    // Functions
    void render();

    void handleEvents();

    void update();

    void playSounds();

    bool measureTime();

};
