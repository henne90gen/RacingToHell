#pragma once

#include "Car.h"
#include "GameObjects.h"

class AICar : public Car
{
public:
	AICar(int Difficulty, int RoadSpeed, sf::Texture& texture);
	~AICar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime, int RoadSpeed);
	void render(sf::RenderWindow& RenderWindow);
	
	void takeDamage(int damage) { setHealth(getHealth() - damage); }

	int getLane() { return _Lane; }
private:
	sf::RectangleShape _HealthBar;
	sf::RectangleShape _HealthBarFrame;

	int _Lane;
};

