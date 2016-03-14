#include "stdafx.h"
#include "Animation/MechLegs.h"


MechLegs::MechLegs(sf::Vector2f pos, sf::Texture & texture) : Animation(pos, texture, 0.1f, 8, 3, 3)
{
}


MechLegs::~MechLegs()
{
}

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
