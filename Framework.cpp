#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework()
{
	//Variablendefinition
	_RenderWindow.create(sf::VideoMode(800, 600, 32U), "Racing to Hell");

	_IsRunning = true;
	_FrameTime = 0;
}

Framework::~Framework()
{

}

void Framework::run()
{
	//Main-Thread
	while (_IsRunning)
	{
		update(_FrameTime);
		handleEvents();
		render();

		measureTime();
	}
}

void Framework::render()
{
	_RenderWindow.clear(sf::Color::White);

	_RenderWindow.display();
}

void Framework::update(float Frametime)
{

}

void Framework::handleEvents()
{
	while (_RenderWindow.pollEvent(_Event))
	{
		if (_Event.type == sf::Event::Closed)
		{
			_IsRunning = false;
			_RenderWindow.close();
		}
	}
}

void Framework::measureTime()
{
	_FrameTime = _Clock.getElapsedTime().asSeconds();
	_LastFPSPrint += _FrameTime;
	_Clock.restart();
	if (_LastFPSPrint > 1) {
		std::cout << "FPS: " << 1 / _FrameTime << std::endl;
		_LastFPSPrint = 0;
	}
}