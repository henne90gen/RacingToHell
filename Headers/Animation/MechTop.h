#pragma once
#include "Animation/Animation.h"

class MechTop : public Animation
{
public:
	/*
		Animation for the top of the boss mech
		@param pos Position of the center of the boss
		@param texture Sprite sheet
	*/
	MechTop(sf::Vector2f pos, sf::Texture& texture);
	~MechTop();

	/*
		See Animation for details
	*/
	virtual void render(sf::RenderWindow & Window);
	
	/*
		See Animation for details
	*/
	virtual void update(float FrameTime);
};

