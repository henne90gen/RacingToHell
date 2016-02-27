#pragma once

#include "GameObject/Car.h"
#include "GameObject/PlayerCar.h"
#include "GameObject/GameObjectContainer.h"
#include "Menu/MainMenu.h"
#include "Menu/PauseMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/GameOverScreen.h"
#include "Menu/LevelUpScreen.h"
#include "HeadsUpDisplay.h"
#include "Level/Level.h"

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	//Functions
	void run();

private:
	//SFML-Graphics
	sf::RenderWindow _RenderWindow;
	sf::Event _Event;
	sf::Clock _Clock;

	//Variables
	float _FrameTime;
	float _LastFPSPrint;
	bool _IsRunning;
	GameObjectContainer _GameObjectContainer;
	GameState _GameState;
	std::vector<sf::Texture*> _CarSkins;
	int _CurrentCarSkinIndex;
	sf::SoundBuffer _MenuMusicBuffer;
	sf::Sound _MenuMusic;
	MainMenu _MainMenu;
	PauseMenu _PauseMenu;
	OptionsMenu _OptionsMenu;
	GameOverScreen _GameOverScreen;
	LevelUpScreen _LevelUpScreen;
	HeadsUpDisplay _HeadsUpDisplay;
	Level _Level;
	float _Score;

	//Functions
	void update(float Frametime);
	void handleEvents();
	void playSounds();
	void setVolume(float Volume);
	void render();
	void measureTime();
	void loadCarSkins();
	void resetGame();
};