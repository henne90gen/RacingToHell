#pragma once
#include "GameObject.h"

class EnergyCanister : public GameObject
{
public:
	EnergyCanister(sf::Vector2f pos);
	~EnergyCanister();

	void update(float FrameTime, int RoadSpeed);
	void handleEvent(sf::Event& Event);
};

