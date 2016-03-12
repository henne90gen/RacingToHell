#pragma once
#include "Animation/Animation.h"

class MechTop : public Animation
{
public:
	MechTop(sf::Vector2f pos, sf::Texture& texture);
	~MechTop();

	virtual void render(sf::RenderWindow & Window);
	virtual void update(float FrameTime);
};

