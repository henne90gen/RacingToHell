#pragma once

#include "Car.h"
#include "GameObjects.h"

class AICar : public Car
{
public:
	AICar();
	~AICar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
	
	void takeDamage(int damage) { setHealth(getHealth() - damage); }

	int getLane() { return _Lane; }
private:
	int _Lane;
};

