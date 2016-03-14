#include "stdafx.h"
#include "Animation/Explosion.h"


Explosion::Explosion(sf::Vector2f pos, sf::Texture & texture, sf::Vector2f movement) : Animation(pos, texture, 0.0075, 81, 9, 9)
{
	_Sprite.setScale(sf::Vector2f(1.1f, 1.1f));
	_Movement = movement;
}


Explosion::~Explosion()
{
}

void Explosion::render(sf::RenderWindow & window)
{
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		window.draw(_Sprite);
}

void Explosion::update(float FrameTime)
{
	if (_AnimState == Animation::Play) {
		_Sprite.setPosition(_Sprite.getPosition() + _Movement * FrameTime);
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			if (!nextFrame()) {
				_AnimState = Animation::Stop;
			}
		}
	}
}
