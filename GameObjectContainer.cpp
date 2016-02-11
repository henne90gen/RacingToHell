#include "stdafx.h"
#include "GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : playerAlive(true)
{
	//Seed
	srand(time(NULL));

	//Spielerauto
	PlayerCar* MainCar = new PlayerCar(100, 800);
	_GameObjects.push_back(MainCar);

	//Frequenz
	_Frequency = 2.0f;
	_BulletFrequency = 1.0f;

	_TimePassed = 0.0f;
	_TimePassedBullet = 0.0f;

	spawnAICar();
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
	//Kollision Spieler
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (i > 0) {
			if (getPlayerCar()->checkForCollision(_GameObjects.at(i))) {
				switch (_GameObjects.at(i)->getType())
				{
				case GameObjects::AI:
					playerAlive = false;
					break;
				default:
					break;
				}
			}
		}
		_GameObjects.at(i)->update(FrameTime);
	}

	//Objekt löschen wenn es sich nicht mehr im Screen befindet
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getHeight() / 2 > SCREENHEIGHT || _GameObjects.at(i)->getPos().y + _GameObjects.at(i)->getHeight() / 2 <= 0 || _GameObjects.at(i)->getPos().x + _GameObjects.at(i)->getWidth() / 2 <= 0 || _GameObjects.at(i)->getPos().x - _GameObjects.at(i)->getWidth() / 2 >= SCREENWIDTH)
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
		_TimePassed += FrameTime - 1 / _Frequency;
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
				if (_GameObjects.at(j)->getType() == GameObjects::AI && i != j && dynamic_cast<AICar*>(_GameObjects.at(i))->getLane() == dynamic_cast<AICar*>(_GameObjects.at(j))->getLane() && dynamic_cast<AICar*>(_GameObjects.at(i))->getSpeed() != dynamic_cast<AICar*>(_GameObjects.at(j))->getSpeed())
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

	//Geschosse spawnen
	if (_TimePassedBullet + FrameTime > 1 / _BulletFrequency)
	{
		_TimePassedBullet += FrameTime - 1 / _BulletFrequency;
		spawnBullet();
	}
	else
	{
		_TimePassedBullet += FrameTime;
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

void GameObjectContainer::resetGameObjects()
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		delete _GameObjects.at(i);
		_GameObjects.at(i) = nullptr;
	}
	_GameObjects = std::vector<GameObject*>();

	//Seed
	srand(time(NULL));

	//Spielerauto
	PlayerCar* MainCar = new PlayerCar(100, 800);
	_GameObjects.push_back(MainCar);

	//Frequenz
	_Frequency = 2;
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

	_GameObjects.push_back(newAiCar);
}

void GameObjectContainer::spawnBullet()
{
	std::vector<AICar*> AICarVector;

	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			AICarVector.push_back(dynamic_cast<AICar*>(_GameObjects.at(i)));
		}
	}

	
	AICar* SelectedCar = AICarVector.at(std::rand() % AICarVector.size());
	
	float Direction;
	if (getPlayerCar()->getPos().x < SelectedCar->getPos().x)
	{
		Direction = std::atanf((SelectedCar->getPos().y - getPlayerCar()->getPos().y) / (SelectedCar->getPos().x - getPlayerCar()->getPos().x)) * 180.0f / PI + 180;
	}
	else if (getPlayerCar()->getPos().x > SelectedCar->getPos().x)
	{
		Direction = std::atanf((getPlayerCar()->getPos().y - SelectedCar->getPos().y) / (getPlayerCar()->getPos().x - SelectedCar->getPos().x)) * 180.0f / PI;
	}
	else
	{
		if (getPlayerCar()->getPos().y > getPlayerCar()->getPos().y)
		{
			Direction = 90;
		}
		else
		{
			Direction = -90;
		}
	}

	Bullet* newBullet = new Bullet(SelectedCar->getPos(), Direction, 100);
	_GameObjects.push_back(newBullet);
}
