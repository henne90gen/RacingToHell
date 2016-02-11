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

	void addObject(GameObject* Object) { _GameObjects.push_back(Object); }

	PlayerCar* getPlayerCar() { return dynamic_cast<PlayerCar*>(_GameObjects.at(0)); }
private:
	std::vector<GameObject*> _GameObjects;
	float _Frequency, _BulletFrequency, _TimePassed, _TimePassedBullet;

	void spawnAICar();
	void spawnBullet();
};