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
	while (_RenderWindow.isOpen())
	{
		update(_FrameTime);
		handleEvents();
		render();

		messureTime();
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
			_RenderWindow.close();
		}

	}
}

void Framework::messureTime()
{

	_FrameTime = _Clock.getElapsedTime().asSeconds();
	_Clock.restart();

	std::cout << "FPS: " << 1 / _FrameTime << std::endl;
}