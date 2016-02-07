#pragma once

#include "stdafx.h"
#include "Car.h"

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
	Car _FirstCar;

	//Functions
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
};