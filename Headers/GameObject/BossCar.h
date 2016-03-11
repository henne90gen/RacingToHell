#pragma once

#include "GameObject\Car.h"
#include "GameObject\GameObjects.h"

class BossCar : public Car
{
public:
	BossCar(sf::Vector2f& Position, int Health, float Speed, sf::Texture& Texture);
	~BossCar();

	void update(float FrameTime, int RoadSpeed, std::vector<GameObject*>& GameObjects);
	void render(sf::RenderWindow& RenderWindow);
private:
	
};