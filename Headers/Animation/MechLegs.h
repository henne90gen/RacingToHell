#pragma once
#include "Animation/Animation.h"

class MechLegs : public Animation
{
public:
	MechLegs(sf::Vector2f pos, sf::Texture & Texture);
	~MechLegs();

	virtual void render(sf::RenderWindow & Window);
	virtual void update(float FrameTime);
};

