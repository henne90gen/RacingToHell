#include "stdafx.h"
#include "Animation/MechTop.h"

MechTop::MechTop(sf::Vector2f pos, sf::Texture & texture) : Animation(pos, texture, 0.1f, 8, 3, 3)
{
}

MechTop::~MechTop()
{
}

void MechTop::render(sf::RenderWindow & window) {
	if (_AnimState == Animation::AnimationState::Play || _AnimState == Animation::AnimationState::Pause)
		window.draw(_Sprite);
}

void MechTop::update(float frameTime)
{
	if (_AnimState == Animation::AnimationState::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			nextFrame();
		}
	}
}
