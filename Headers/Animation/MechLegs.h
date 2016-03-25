#pragma once
#include "Animation/Animation.h"

class MechLegs : public Animation
{
public:
	/*
		Animation for the legs of the boss mech
		@param pos Position of the center of the boss
		@param texture Sprite sheet
	*/
	MechLegs(sf::Vector2f pos, sf::Texture & texture) : Animation(pos, texture, 0.1f, 8, 3, 3) {}
	~MechLegs() {}

	/*
		See Animation for details
	*/
	virtual void render(sf::RenderWindow & window);

	/*
		See Animation for details
	*/
	virtual void update(float frameTime);
};

