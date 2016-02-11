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

	//AI-Autos auf Kollision prüfen
	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			for (unsigned int j = 1; j < _GameObjects.size(); j++)
			{
				if (i != j && dynamic_cast<AICar*>(_GameObjects.at(i))->getLane() == dynamic_cast<AICar*>(_GameObjects.at(j))->getLane() && dynamic_cast<AICar*>(_GameObjects.at(i))->getSpeed() != dynamic_cast<AICar*>(_GameObjects.at(j))->getSpeed())
				{
					if (std::abs(_GameObjects.at(i)->getPos().y - _GameObjects.at(j)->getPos().y) < _GameObjects.at(i)->getHeight() + 20)
					{
						int minSpeed = std::min({ dynamic_cast<AICar*>(_GameObjects.at(i))->getSpeed(), dynamic_cast<AICar*>(_GameObjects.at(j))->getSpeed() });
						dynamic_cast<AICar*>(_GameObjects.at(i))->setSpeed(minSpeed);
						dynamic_cast<AICar*>(_GameObjects.at(j))->setSpeed(minSpeed);
					}
				}
			}
		}
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

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			if (dynamic_cast<AICar*>(_GameObjects.at(i))->getLane() == newAiCar->getLane() && _GameObjects.at(i)->getPos().y < _GameObjects.at(i)->getHeight() / 2 + 20)
			{
				delete newAiCar;
				newAiCar = nullptr;
				return;
			}
		}
	}

	addObject(newAiCar);
}