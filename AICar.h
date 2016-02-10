#pragma once

#include "Car.h"

class AICar : Car
{
public:
	AICar();
	~AICar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
};

