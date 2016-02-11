#pragma once

#include "stdafx.h"
#include "Car.h"
#include "PlayerCar.h"
#include "GameObjectContainer.h"
#include "Menu.h"
#include "GameOverScreen.h"
#include "HeadsUpDisplay.h"
#include "Level.h"

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	//Functions
	void run();

	enum GameState {
		Running, Pausing, GameOver, Exiting
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
	int _Score;

	//Functions
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
	void showMenu();
	void showGameOverScreen();
	void loadCarSkins();
	void resetGame();
};