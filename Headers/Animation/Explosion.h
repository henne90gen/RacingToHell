#pragma once
#include "Animation/Animation.h"

class Explosion : public Animation
{
public:
	Explosion(sf::Vector2f pos, sf::Texture & Texture, sf::Vector2f Movement);
	~Explosion();

	virtual void render(sf::RenderWindow & Window);
	virtual void update(float FrameTime);

private:
	sf::Vector2f _Movement;
};

