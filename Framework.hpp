#pragma once

#include "stdafx.h"
#include "Car.h"
#include "PlayerCar.h"
#include "Cars.h"
#include "GameObjectContainer.h"
#include "Menu.h"

class Framework
{
public:
	//Constructor, Destructor
	Framework();
	~Framework();

	//Functions
	void run();

	enum GameState {
		Running, Pausing, Exiting
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
	std::vector<sf::Sprite*> _CarSkins;
	int _CurrentCarSkinIndex;

	//Functions
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
	void showMenu();
	void loadCarSkins();
};