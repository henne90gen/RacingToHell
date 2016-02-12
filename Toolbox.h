#pragma once
#include "GameObject.h"

class Toolbox : public GameObject
{
public:
	Toolbox(sf::Vector2f pos);
	~Toolbox();

	void update(float FrameTime);
	void handleEvent(sf::Event& Event);
};

