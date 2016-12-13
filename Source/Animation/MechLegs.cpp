#include "stdafx.h"
#include "Animation/MechLegs.h"

void MechLegs::render(sf::RenderWindow & window) {
	if (_AnimationState == Animation::AnimationState::Play || _AnimationState == Animation::AnimationState::Pause)
		window.draw(_Sprite);
}

void MechLegs::update(float frameTime)
{
	if (_AnimationState == Animation::AnimationState::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			nextFrame();
		}
	}
}
