#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameState(GameState::Pausing)
{
	//Definition of variables
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell");
	loadCarSkins();
	
}

Framework::~Framework()
{
	for (unsigned int i = 0; i < _CarSkins.size(); i++) {
		delete _CarSkins.at(i);
		_CarSkins.at(i) = nullptr;
	}
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
	MenuResult result = _Menu.render(_RenderWindow, *_CarSkins.at(_CurrentCarSkinIndex));
	switch (result) {
	case MenuResult::Resume:
		_GameState = GameState::Running;
		break;
	case MenuResult::PreviousSkin:
		//TODO: Switch to previous skin
		_CurrentCarSkinIndex--;
		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		}
		_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
		break;
	case MenuResult::NextSkin:
		//TODO: Switch to next skin
		_CurrentCarSkinIndex++;
		if (_CurrentCarSkinIndex >= _CarSkins.size()) {
			_CurrentCarSkinIndex = 0;
		}
		_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
		break;
	case MenuResult::Exit:
		_GameState = GameState::Exiting;
		break;
	}
}

void Framework::loadCarSkins()
{
	//TODO: For loop with strings
	sf::Texture* texture = new sf::Texture();
	if (texture->loadFromFile("Resources/car.png")) {
		_CarSkins.push_back(texture);
	}

	/*
	if (texture->loadFromFile("Resources/truck.png")) {
		_CarSkins.push_back(texture);
	}
	*/
}
