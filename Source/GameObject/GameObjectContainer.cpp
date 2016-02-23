#include "stdafx.h"
#include "GameObject\GameObjectContainer.h"

GameObjectContainer::GameObjectContainer() : _PlayerBulletSpeed(600), _AIBulletSpeed(300)
{
	//Seed
	srand(time(NULL));

	//Variablen
	resetGameObjects(0);
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
					_PlayerAlive = false;
					break;
				case GameObjects::BulletObjectAI:
					getPlayerCar()->takeDamage();
					deleteObject(i);
					i--;
					break;
				case GameObjects::Canister:
					getPlayerCar()->addEnergy();
					deleteObject(i);
					i--;
					break;
				case GameObjects::Tools:
					getPlayerCar()->addHealth();
					deleteObject(i);
					i--;
					break;
				}
			}
		}
		else {
			if (getPlayerCar()->getHealth() <= 0 || getPlayerCar()->getEnergy() <= 0) {
				_PlayerAlive = false;
			}
		}
		_GameObjects.at(i)->update(FrameTime);
	}

	//Objekt löschen wenn es sich nicht mehr im Screen befindet
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getPos().y - _GameObjects.at(i)->getHeight() / 2 > SCREENHEIGHT || _GameObjects.at(i)->getPos().y + _GameObjects.at(i)->getHeight() / 2 <= 0 || _GameObjects.at(i)->getPos().x + _GameObjects.at(i)->getWidth() / 2 <= 0 || _GameObjects.at(i)->getPos().x - _GameObjects.at(i)->getWidth() / 2 >= SCREENWIDTH)
		{
			deleteObject(i);
			i--;
		}
	}

	//EnergyCanister spawnen
	if (_TimePassedCanister + FrameTime > 1 / _CanisterFrequency) 
	{
		_TimePassedCanister += FrameTime - 1 / _CanisterFrequency;
		EnergyCanister* canister = new EnergyCanister(sf::Vector2f(std::rand() % 3 * 150 + 150, -25));
		_GameObjects.push_back(canister);
	}
	else {
		_TimePassedCanister += FrameTime;
	}

	//Toolbox spawnen
	if (_TimePassedToolbox + FrameTime > 1 / _ToolboxFrequency)
	{
		_TimePassedToolbox += FrameTime - 1 / _ToolboxFrequency;
		_ToolboxFrequency = (float)(std::rand() % 150) / 1000.0f;
		Toolbox* toolbox = new Toolbox(sf::Vector2f(std::rand() % 3 * 150 + 150, -10));
		_GameObjects.push_back(toolbox);
	}
	else {
		_TimePassedToolbox += FrameTime;
	}

	//AI-Autos spawnen
	if (_TimePassedCar + FrameTime > 1 / _CarFrequency)
	{
		_TimePassedCar += FrameTime - 1 / _CarFrequency;
		spawnAICar();
	}
	else
	{
		_TimePassedCar += FrameTime;
	}

	//AI-Autos auf Kollision prüfen
	for (unsigned int i = 1; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI)
		{
			for (unsigned int j = i + 1; j < _GameObjects.size(); j++)
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
				else if (_GameObjects.at(j)->getType() == GameObjects::BulletObjectPlayer)
				{
					if (_GameObjects.at(i)->getSprite().getGlobalBounds().intersects(_GameObjects.at(j)->getSprite().getGlobalBounds()))
					{
						dynamic_cast<AICar*>(_GameObjects.at(i))->takeDamage(getPlayerCar()->getBulletdamage());
						deleteObject(j);
						break;
					}
				}
			}
		}
	}

	//kaputte Autos löschen
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		if (_GameObjects.at(i)->getType() == GameObjects::AI && dynamic_cast<AICar*>(_GameObjects.at(i))->getHealth() <= 0)
		{
			_CarScore += 100;
			deleteObject(i);
			i--;
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

	//Prüfen ob Spieler geschossen hat
	if (getPlayerCar()->shotBullet() != 360.0f)
	{
		Bullet* newBullet = new Bullet(getPlayerCar()->getPos(), getPlayerCar()->shotBullet(), _PlayerBulletSpeed, GameObjects::BulletObjectPlayer);
		_GameObjects.push_back(newBullet);

		getPlayerCar()->resetShotBullet();
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

void GameObjectContainer::resetGameObjects(int SelectedCar)
{
	for (unsigned int i = 0; i < _GameObjects.size(); i++)
	{
		delete _GameObjects.at(i);
		_GameObjects.at(i) = nullptr;
	}
	_GameObjects.clear();

	//Spielerauto
	PlayerCar* MainCar = new PlayerCar(SelectedCar);
	_GameObjects.push_back(MainCar);

	//Frequenz
	_CarFrequency = 2;
	_BulletFrequency = 1.0f;
	_CanisterFrequency = 0.3f;
	_ToolboxFrequency = (float)(std::rand() % 150) / 1000.0f;

	_TimePassedCar = 0.0f;
	_TimePassedBullet = 0.0f;
	_TimePassedCanister = 0.0f;
	_TimePassedToolbox = 0.0f;

	_PlayerAlive = true;
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

	if (AICarVector.size() == 0)
		return;

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
		if (getPlayerCar()->getPos().y > SelectedCar->getPos().y)
		{
			Direction = 90;
		}
		else
		{
			Direction = -90;
		}
	}

	Bullet* newBullet = new Bullet(SelectedCar->getPos(), Direction, _AIBulletSpeed, GameObjects::BulletObjectAI);
	_GameObjects.push_back(newBullet);
}

void GameObjectContainer::deleteObject(unsigned int id)
{
	delete _GameObjects.at(id);
	_GameObjects.at(id) = nullptr;
	_GameObjects.erase(_GameObjects.begin() + id);
}
