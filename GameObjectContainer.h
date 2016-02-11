#pragma once

#include "GameObject.h"
#include "GameObjects.h"
#include "AICar.h"
#include "PlayerCar.h"
#include "Bullet.h"
#include "EnergyCanister.h"

class GameObjectContainer
{
public:
	GameObjectContainer();
	~GameObjectContainer();

	void update(float FrameTime);
	void render(sf::RenderWindow& RenderWindow);
	void handleEvents(sf::Event& Event);

	PlayerCar* getPlayerCar() { return dynamic_cast<PlayerCar*>(_GameObjects.at(0)); }
	bool playerIsAlive() { return _PlayerAlive; }

	void resetGameObjects();

	int getCarScore() { int result = _CarScore; _CarScore = 0; return result; }

private:
	std::vector<GameObject*> _GameObjects;
	float _CarFrequency, _BulletFrequency, _TimePassedCar, _TimePassedBullet, _CanisterFrequency, _TimePassedCanister;
	int _CarScore;

	void spawnAICar();
	bool _PlayerAlive;
	void spawnBullet();
	void deleteObject(unsigned int id);
};