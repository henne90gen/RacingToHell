#include "stdafx.h"
#include "Animation/MechTop.h"

MechTop::MechTop(sf::Vector2f pos, sf::Texture & texture) : Animation(pos, texture, 0.1f, 8, 3, 3)
{
}

MechTop::~MechTop()
{
}

void MechTop::render(sf::RenderWindow & Window) {
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		Window.draw(_Sprite);
}

void MechTop::update(float FrameTime)
{
	if (_AnimState == Animation::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			nextSprite();
		}
	}
}
