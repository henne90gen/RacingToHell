#include "stdafx.h"
#include "Framework.hpp"

Framework::Framework() : _FrameTime(0), _IsRunning(true), _GameState(GameState::MainMenu)
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
		
		switch (_GameState) {
		case GameState::Running:
			update(_FrameTime);
			handleEvent();
			break;
		case GameState::MainMenu:
			_Level.update(_FrameTime);
			handleEventMenu();
			break;
		case GameState::Pausing:
			handleEventMenu();
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
	if (_GameState == GameState::Running) {
		_HeadsUpDisplay.render(_RenderWindow);
	}
	else if (_GameState == GameState::MainMenu || _GameState == GameState::Pausing) {
		_Menu.render(_RenderWindow, _CurrentCarSkinIndex, _GameState == GameState::Pausing, _HeadsUpDisplay);
	}
	else if (_GameState == GameState::GameOver) {
		_GameOverScreen.render(_RenderWindow, _Score);
	}
	_RenderWindow.display();
}

void Framework::update(float FrameTime)
{
	_Level.update(FrameTime);
	_GameObjectContainer.update(FrameTime, _Level.getRoadSpeed());
	if (!_GameObjectContainer.playerIsAlive()) {
		_GameState = GameState::GameOver;
	}
	_HeadsUpDisplay.update(_Score, _GameObjectContainer.getPlayerCar()->getHealth(), _GameObjectContainer.getPlayerCar()->getMaxHealth(), _GameObjectContainer.getPlayerCar()->getEnergy(), _GameObjectContainer.getPlayerCar()->getMaxEnergy());
	_Score += _GameObjectContainer.getCarScore();
	_Score += 10 * FrameTime;
}

void Framework::handleEvent()
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

void Framework::handleEventMenu()
{
	while (_RenderWindow.pollEvent(_Event)) {
		_Menu.checkMouseHover(_RenderWindow);
		if (_Event.type == sf::Event::MouseButtonPressed) {
			sf::Vector2f MousePos = sf::Vector2f(_Event.mouseButton.x, _Event.mouseButton.y);
			for (int i = 0; i < _Menu.getMenuItems().size(); i++) {
				sf::FloatRect rect = _Menu.getMenuItems()[i]->getRect();
				if (MousePos.y > rect.top && MousePos.y < rect.top + rect.height && MousePos.x > rect.left && MousePos.x < rect.left + rect.width) {
					switch (_Menu.getMenuItems()[i]->getAction()) {
					case MenuResult::Resume:
						_Clock.restart();
						_GameState = GameState::Running;
						break;
					case MenuResult::Nothing:
						_GameState = GameState::Pausing;
						break;
					case MenuResult::PreviousSkin:
						if (_GameState == GameState::MainMenu) {
							_CurrentCarSkinIndex--;
							if (_CurrentCarSkinIndex < 0) {
								_CurrentCarSkinIndex = _CarSkins.size() - 1;
							}
							_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
							_GameObjectContainer.getPlayerCar()->setStats(_CurrentCarSkinIndex);
						}
						break;
					case MenuResult::NextSkin:
						if (_GameState == GameState::MainMenu) {
							_CurrentCarSkinIndex++;
							if (_CurrentCarSkinIndex >= _CarSkins.size()) {
								_CurrentCarSkinIndex = 0;
							}
							_GameObjectContainer.getPlayerCar()->setSkin(_CarSkins.at(_CurrentCarSkinIndex));
							_GameObjectContainer.getPlayerCar()->setStats(_CurrentCarSkinIndex);
						}
						break;
					case MenuResult::Exit:
						_GameState = GameState::Exiting;
						break;
					}
				}
			}
		}
		else if (_Event.type == sf::Event::Closed) {
			_GameState = GameState::Exiting;
		}
	}
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
		_GameState = GameState::MainMenu;
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