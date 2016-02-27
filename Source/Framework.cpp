#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameState(GameState::Main)
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
		render();

		handleEvents();

		playSounds();

		switch (_GameState) {
		case GameState::Running:
			update(_FrameTime);
			break;
		case GameState::Main:
			_Level.update(_FrameTime);
			break;
		case GameState::Pause:
			_Level.update(_FrameTime);
			break;
		case GameState::Options:
			_Level.update(_FrameTime);
			handleEventOptions();
			break;
		case GameState::GameOver:
			handleEventGameOver();
			break;
		case GameState::Exiting:
			_IsRunning = false;
			_RenderWindow.close();
			break;
		}

		measureTime();

	}
}

void Framework::render()
{
	_RenderWindow.clear(sf::Color::Cyan);
	_Level.render(_RenderWindow);
	_GameObjectContainer.render(_RenderWindow);
	
	switch (_GameState) {
	case GameState::Running:
		_HeadsUpDisplay.render(_RenderWindow);
		break;
	case GameState::Main:
		_MainMenu.render(_RenderWindow, _CurrentCarSkinIndex);
		break;
	case GameState::Pause:
		_PauseMenu.render(_RenderWindow);
		break;
	case GameState::Options:
		_OptionsMenu.render(_RenderWindow);
		break;
	case GameState::GameOver:
		_GameOverScreen.render(_RenderWindow, _Score);
		break;
	}

	_RenderWindow.display();
}

void Framework::playSounds() {
	if (_GameState == GameState::Running) {
		_MainMenu.stopMusic();
		_Level.playMusic();
	}
	else if (_GameState == GameState::Main || _GameState == GameState::Pause) {
		_Level.pauseMusic();
		_MainMenu.playMusic();
	}
	else if (_GameState == GameState::GameOver) {
		_Level.stopMusic();
		//_GameOverScreen.playSounds();
	}
}

void Framework::update(float FrameTime)
{
	_Level.update(FrameTime);
	_GameObjectContainer.update(FrameTime);
	if (!_GameObjectContainer.playerIsAlive()) {
		_GameState = GameState::GameOver;
	}
	_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar()->getHealth(), _GameObjectContainer.getPlayerCar()->getMaxHealth(), _GameObjectContainer.getPlayerCar()->getEnergy(), _GameObjectContainer.getPlayerCar()->getMaxEnergy());
	_Score += _GameObjectContainer.getCarScore();
	_Score += 10 * FrameTime;
}

void Framework::handleEvents()
{
	switch (_GameState) {
	case GameState::Running:
		while (_RenderWindow.pollEvent(_Event))
		{
			if (_Event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					_GameState = GameState::Pause;
				}
				else {
					_GameObjectContainer.handleEvents(_Event);
				}
			}
			else if (_Event.type == sf::Event::KeyReleased || _Event.type == sf::Event::MouseButtonPressed) {
				_GameObjectContainer.handleEvents(_Event);
			}
			else if (_Event.type == sf::Event::Closed) {
				_GameState = GameState::Exiting;
			}
		}
		break;
	case GameState::Pause:
		_GameState = _PauseMenu.handleEvents(_RenderWindow);
		break;
	case GameState::Main:
		_GameState = _MainMenu.handleEvents(_RenderWindow, _CurrentCarSkinIndex);
		if (_GameState == GameState::Running) { _Clock.restart(); }
		if (_CurrentCarSkinIndex < 0) {
			_CurrentCarSkinIndex = _CarSkins.size() - 1;
		} 
		else if (_CurrentCarSkinIndex >= _CarSkins.size()) {
			_CurrentCarSkinIndex = 0;
		}
		_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
		_GameObjectContainer.getPlayerCar()->setStats(_CurrentCarSkinIndex);
		break;
	case GameState::Options:
		
		break;
	}
}

void Framework::handleEventOptions()
{
	
}

void Framework::handleEventGameOver()
{
	MenuResult result = MenuResult::Nothing;
	while (_RenderWindow.pollEvent(_Event)) {
		if (_Event.type == sf::Event::KeyPressed) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				result = MenuResult::Restart;
			}
		}
		else if (_Event.type == sf::Event::Closed) {
			result = MenuResult::Exit;
		}
	}
	switch (result) {
	case MenuResult::Restart:
		resetGame();
		_GameState = GameState::Main;
		break;
	case MenuResult::Exit:
		_GameState = GameState::Exiting;
		break;
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

void Framework::loadCarSkins()
{
	for (unsigned int i = 1; i < 7; i++) {
		sf::Texture* texture = new sf::Texture();
		if (texture->loadFromFile("Resources/Texture/PlayerCar/playercar" + std::to_string(i) + ".png")) {
			_CarSkins.push_back(texture);
		}
	}
}

void Framework::resetGame() 
{
	_Clock.restart();
	_Score = 0;
	_GameObjectContainer.resetGameObjects(_CurrentCarSkinIndex);
}