#pragma once

#include "GameObject.h"
#include "GameObjects.h"

class Bullet : public GameObject
{
public:
	Bullet(sf::Vector2f Position, float _Direction, int Speed, GameObjects Type, sf::Texture& texture);
	~Bullet();

	void update(float FrameTime, int RoadSpeed);
	void handleEvent(sf::Event& Event);

private:
	float _Speed, _Direction;
};