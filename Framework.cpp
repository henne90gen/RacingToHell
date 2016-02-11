#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework()
{
	//Definition of variables
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell");
	_IsRunning = true;
	_FrameTime = 0;
}

Framework::~Framework()
{
}

void Framework::run()
{
	//Main thread
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
	_RenderWindow.clear(sf::Color::Cyan);
	_GameObjectContainer.render(_RenderWindow);
	_RenderWindow.display();
}

void Framework::update(float FrameTime)
{
	_GameObjectContainer.update(FrameTime);
}

void Framework::handleEvents()
{
	while (_RenderWindow.pollEvent(_Event))
	{
		if (_Event.type == sf::Event::KeyPressed || _Event.type == sf::Event::KeyReleased) {
			_GameObjectContainer.handleEvents(_Event);
		} else if (_Event.type == sf::Event::Closed) {
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