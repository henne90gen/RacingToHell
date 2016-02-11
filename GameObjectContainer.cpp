#include "stdafx.h"
#include "GameObjectContainer.h"

GameObjectContainer::GameObjectContainer()
{
	//Seed
	srand(time(NULL));

	//Spielerauto
	PlayerCar* MainCar = new PlayerCar(100, 800);
	addObject(MainCar);

	//Frequenz
	_Frequency = 2;
}

GameObjectContainer::~GameObjectContainer()
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		delete _GameObjects.at(i);
		_GameObjects.at(i) = nullptr;
	}
}

void GameObjectContainer::update(float FrameTime)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (i > 0) {
			dynamic_cast<PlayerCar*>(_GameObjects.at(0))->checkForCollision(_GameObjects.at(i));
		}
		_GameObjects.at(i)->update(FrameTime);
	}

	//Objekt löschen wenn es sich nicht mehr im Screen befindet
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getWidth() / 2 > SCREENHEIGHT)
		{
			delete _GameObjects.at(i);
			_GameObjects.at(i) = nullptr;
			_GameObjects.erase(_GameObjects.begin() + i);
			i--;
		}
	}

	//neue AI-Autos spawnen
	if (_TimePassed + FrameTime > 1 / _Frequency)
	{
		_TimePassed = _TimePassed + FrameTime - 1 / _Frequency;
		spawnAICar();
	}
	else
	{
		_TimePassed += FrameTime;
	}
}

void GameObjectContainer::render(sf::RenderWindow& RenderWindow)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		_GameObjects.at(i)->render(RenderWindow);
	}
}

void GameObjectContainer::handleEvents(sf::Event& Event)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		_GameObjects.at(i)->handleEvent(Event);
	}
}

void GameObjectContainer::spawnAICar()
{
	AICar* newAiCar = new AICar();
	addObject(newAiCar);
}
