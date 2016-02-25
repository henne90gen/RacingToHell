#pragma once

#include "stdafx.h"
#include "GameObject\Car.h"
#include "GameObject\PlayerCar.h"
#include "GameObject\GameObjectContainer.h"
#include "Menu\Menu.h"
#include "Menu\GameOverScreen.h"
#include "HeadsUpDisplay.h"
#include "Level\Level.h"

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	//Functions
	void run();

	enum GameState {
		Running, LevelUp, MainMenu, Pausing, GameOver, Exiting
	};

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
	Menu _Menu;
	std::vector<sf::Texture*> _CarSkins;
	int _CurrentCarSkinIndex;
	GameOverScreen _GameOverScreen;
	HeadsUpDisplay _HeadsUpDisplay;
	Level _Level;
	float _Score;

	//Functions
	void update(float Frametime);
	void handleEvent();
	void handleEventMenu();
	void handleEventGameOver();
	void render();
	void measureTime();
	void loadCarSkins();
	void resetGame();
};