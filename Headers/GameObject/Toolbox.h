#pragma once
#include "GameObject/GameObject.h"

class Toolbox : public GameObject
{
public:
	Toolbox(sf::Vector2f pos);
	~Toolbox();

	void update(float FrameTime, int RoadSpeed);
	void handleEvent(sf::Event& Event);
};

