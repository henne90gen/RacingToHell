#include "stdafx.h"
#include "Animation/MechLegs.h"

void MechLegs::render(sf::RenderWindow & window) {
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		window.draw(_Sprite);
}

void MechLegs::update(float frameTime)
{
	if (_AnimState == Animation::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			nextFrame();
		}
	}
}
