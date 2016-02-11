#pragma once

#include "stdafx.h"
#include "Car.h"
#include "PlayerCar.h"
#include "GameObjectFactory.h"
#include "Cars.h"
#include "GameObjectContainer.h"

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

	//Functions
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
};