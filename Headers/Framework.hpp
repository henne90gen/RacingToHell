#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/GameObjectContainer.h"
#include "Menu/MainMenu.h"
#include "Menu/PauseMenu.h"
#include "Menu/HighscoreMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/GameOverScreen.h"
#include "Menu/LoadingScreen.h"
#include "Menu/LevelUpScreen.h"
#include "HeadsUpDisplay.h"
#include "Level/Level.h"

#include <sstream>
#include <fstream>

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	/*
		Starts the main game loop
	*/
	void run();

private:
	//SFML-Graphics
	sf::RenderWindow _RenderWindow;
	sf::Event _Event;
	sf::Clock _Clock;

	//Variables
	float _FrameTime, _LastFPSPrint, _LastFPSCheck, _Score, _FPS;
	bool _IsRunning;
	int _CurrentCarSkinIndex;

	//GameObjects
	GameState _GameState;
	GameObjectContainer _GameObjectContainer;
	std::vector<sf::Texture*> _CarSkins;

	//Level
	Level _Level;

	//Music
	sf::SoundBuffer _MenuMusicBuffer;
	sf::Sound _MenuMusic;

	//Menus and Screens
	MainMenu _MainMenu;
	PauseMenu _PauseMenu;
	HighscoreMenu _HighscoreMenu;
	OptionsMenu _OptionsMenu;
	GameOverScreen _GameOverScreen;
	LoadingScreen _LoadingScreen;
	LevelUpScreen _LevelUpScreen;
	HeadsUpDisplay _HeadsUpDisplay;
	
	//Functions
	void render();
	void handleEvents();
	void update();
	void playSounds();
	bool measureTime();
	void load();
	void resetGame();
	void setVolume(float Volume);
	void setMouseVisible(bool visible);
};