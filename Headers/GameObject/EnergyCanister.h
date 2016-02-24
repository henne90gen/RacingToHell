#pragma once
#include "GameObject.h"

class EnergyCanister : public GameObject
{
public:
	EnergyCanister(sf::Vector2f pos, sf::Texture& texture);
	~EnergyCanister();

	void update(float FrameTime, int RoadSpeed);
	void handleEvent(sf::Event& Event);
};

