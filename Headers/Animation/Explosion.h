#pragma once
#include "Animation/Animation.h"

class Explosion : public Animation
{
public:
	Explosion(sf::Vector2f pos);
	~Explosion();

	virtual void render(sf::RenderWindow & Window);
	virtual void update(float FrameTime);
};

