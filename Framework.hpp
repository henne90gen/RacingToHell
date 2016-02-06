#ifndef FRAMEWORK
#define FRAMEWORK

#include "stdafx.h"
#include <iostream>
#include <SFML\Graphics.hpp>

class Framework
{
public:
	//Konstruktor, Dekonstruktor
	Framework();
	~Framework();

	//Funktionen
	void run();
private:
	//SFML-Graphics
	sf::RenderWindow _RenderWindow;
	sf::Event _Event;
	sf::Clock _Clock;

	//Variablen
	float _FrameTime;
	bool _IsRunning;

	//Funktionen
	void update(float Frametime);
	void handleEvents();
	void render();
	void measureTime();
};

#endif