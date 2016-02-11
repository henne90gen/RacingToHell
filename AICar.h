#pragma once

#include "Car.h"
#include <time.h>

class AICar : public Car
{
public:
	AICar();
	~AICar();

	void handleEvent(sf::Event& Event);
	void update(float FrameTime);
};

