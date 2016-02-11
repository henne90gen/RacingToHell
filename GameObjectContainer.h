#pragma once

#include "GameObject.h"
#include "AICar.h"
#include "PlayerCar.h"

class GameObjectContainer
{
public:
	GameObjectContainer();
	~GameObjectContainer();

	void update(float FrameTime);
	void render(sf::RenderWindow& RenderWindow);
	void handleEvents(sf::Event& Event);

	void addObject(GameObject* Object) { _GameObjects.push_back(Object); }
private:
	std::vector<GameObject*> _GameObjects;
	float _Frequency, _TimePassed;

	void spawnAICar();
};