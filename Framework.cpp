#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameState(GameState::Pausing)
{
	//Definition of variables
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell");
}

Framework::~Framework()
{
}

void Framework::run()
{
	//Main thread
	while (_IsRunning)
	{
		switch (_GameState) {
		case GameState::Running:
		update(_FrameTime);
		handleEvents();
		render();
		measureTime();
			break;
		case GameState::Pausing:
			showMenu();
			break;
		case GameState::Exiting:
			_IsRunning = false;
			_RenderWindow.close();
			break;
	}
		
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
		if (_Event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				_GameState = GameState::Pausing;
			}
			else {
				_GameObjectContainer.handleEvents(_Event);
			}
		} else if (_Event.type == sf::Event::KeyReleased) {
			_GameObjectContainer.handleEvents(_Event);
		} else if (_Event.type == sf::Event::Closed) {
			_GameState = GameState::Exiting;
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

void Framework::showMenu() {
	MenuResult result = _Menu.render(_RenderWindow, sf::Sprite());
	switch (result) {
	case MenuResult::Resume:
		_GameState = GameState::Running;
		break;
	case MenuResult::PreviousSkin:
		//TODO: Switch to previous skin
		break;
	case MenuResult::NextSkin:
		//TODO: Switch to next skin
		break;
	case MenuResult::Exit:
		_GameState = GameState::Exiting;
		break;
	}
}