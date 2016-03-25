#pragma once
#include "Animation/Animation.h"

class Explosion : public Animation
{
public:
	/*
		Explosion animation
		@param pos Position of the center of the explosion
		@param texture Sprite sheet
		@param movement Vector that is going to be applied to the explosion to make it move
	*/
	Explosion(sf::Vector2f pos, sf::Texture & texture, sf::Vector2f movement);
	~Explosion() {}

	/*
		See Animation for details
	*/
	virtual void render(sf::RenderWindow & Window);

	/*
		See Animation for details
	*/
	virtual void update(float FrameTime);

private:
	sf::Vector2f _Movement;
};

