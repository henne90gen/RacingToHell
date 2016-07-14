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

#include <sstream>
#include <fstream>
#include <thread>

class Framework
{
public:
	/*
		The Framework takes care of all the major aspects of the game.
		Rendering, updating and event handling are all managed in here.
	*/
	Framework();
	~Framework();

	/*
		Starts the main game loop
	*/
	void run();

private:
	// SFML-Graphics
	sf::RenderWindow _RenderWindow;
	sf::Event _Event;
	sf::Clock _Clock;

	// Variables
	GameMode _GameMode;
	float _FrameTime, _LastFPSPrint, _LastFPSCheck, _Score, _FPS;
	bool _IsRunning;
	int _CurrentCarSkinIndex;

	// GameObjects
	GameState _GameState;
	GameObjectContainer _GameObjectContainer;
	std::vector<std::shared_ptr<sf::Texture>> _CarSkins;

	// TODO: Multi
	//MPGameObjectContainer _MPGameObjectContainer;

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
	// TODO: Multi
	//MultiplayerMenu _MultiplayerMenu;
	//MultiplayerLobby _MultiplayerLobby;
	//PauseMultiplayerMenu _PauseMultiplayerMenu;
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
	void load();
	void resetGame();
	void setVolume(float Volume);
	void setMouseVisible(bool visible);
	void setDifficulty(int Difficulty);
	void setGameMode(GameMode gameMode);
	void addScore();
};
