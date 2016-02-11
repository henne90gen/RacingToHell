#pragma once

#include "GameObject.h"
#include "GameObjects.h"

class Bullet : public GameObject
{
public:
	Bullet(sf::Vector2f Position, float _Direction, int Speed);
	~Bullet();

	void update(float FrameTime);
	void handleEvent(sf::Event& Event);
private:
	int _Speed, _Direction;
};