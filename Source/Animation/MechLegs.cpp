#include "stdafx.h"
#include "Animation/MechLegs.h"


MechLegs::MechLegs(sf::Vector2f pos, sf::Texture & Texture) : Animation(pos, Texture, 0.01f, 8, 3, 3)
{
}


MechLegs::~MechLegs()
{
}

void MechLegs::render(sf::RenderWindow & Window) {
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		Window.draw(_Sprite);
}

void MechLegs::update(float FrameTime)
{
	
}
