#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameState(GameState::Pausing)
{
	_RenderWindow.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32U), "Racing to Hell");
	_RenderWindow.setFramerateLimit(300);
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
		case GameState::GameOver:
			showGameOverScreen();
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
	_Level.render(_RenderWindow);
	_GameObjectContainer.render(_RenderWindow);
	_HeadsUpDisplay.render(_RenderWindow);
	_RenderWindow.display();
}

void Framework::update(float FrameTime)
{
	_Level.update(FrameTime);
	_GameObjectContainer.update(FrameTime);
	if (!_GameObjectContainer.playerIsAlive()) {
		_GameState = GameState::GameOver;
	}
	_HeadsUpDisplay.update(_GameObjectContainer.getPlayerCar()->getHealth(), _Score, _GameObjectContainer.getPlayerCar()->getEnergy(), _GameObjectContainer.getPlayerCar()->getMaxEnergy());
	_Score += _GameObjectContainer.getCarScore();
	_Score += 10*FrameTime;
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
		} else if (_Event.type == sf::Event::KeyReleased || _Event.type == sf::Event::MouseButtonPressed) {
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

void Framework::showMenu() 
{
	MenuResult result = _Menu.render(_RenderWindow, *_CarSkins.at(_CurrentCarSkinIndex));
	switch (result) {
	case MenuResult::Nothing:
		_GameState = GameState::Pausing;
		break;
	case MenuResult::Resume:
		_Clock.restart();
		_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
		_GameState = GameState::Running;
		break;
	case MenuResult::PreviousSkin:
		_CurrentCarSkinIndex--;
		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		}
		_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
		break;
	case MenuResult::NextSkin:
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

void Framework::showGameOverScreen()
{
	MenuResult result = _GameOverScreen.render(_RenderWindow, _Score, _GameObjectContainer);
	switch (result) {
	case MenuResult::Restart:
		resetGame();
		_GameState = GameState::Pausing;
		break;
	case MenuResult::Exit:
		_GameState = GameState::Exiting;
		break;
	}
}

void Framework::loadCarSkins()
{
	std::vector<std::string> skins = { "1", "2", "3", "4", "5" };
	for (unsigned int i = 0; i < skins.size(); i++) {
		sf::Texture* texture = new sf::Texture();
		if (texture->loadFromFile("Resources/playercar" + skins.at(i) + ".png")) {
			_CarSkins.push_back(texture);
		}
	}
}

void Framework::resetGame() 
{
	_Clock.restart();
	_Score = 0;
	_GameObjectContainer.resetGameObjects();
}