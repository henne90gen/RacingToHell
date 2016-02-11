#pragma once

#include "GameObject.h"
#include "GameObjects.h"
#include "AICar.h"
#include "PlayerCar.h"
#include "Bullet.h"

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

private:
	std::vector<GameObject*> _GameObjects;
	float _Frequency, _BulletFrequency, _TimePassed, _TimePassedBullet;

	void spawnAICar();
	bool _PlayerAlive;
	void spawnBullet();
};