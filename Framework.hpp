#pragma once

#include "stdafx.h"
#include "PlayerCar.h"
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
	GameState _GameState;
	PlayerCar _FirstCar;
	Menu _Menu;

	//Functions
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
	void showMenu();
};