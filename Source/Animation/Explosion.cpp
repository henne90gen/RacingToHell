#include "stdafx.h"
#include "Animation/Explosion.h"


Explosion::Explosion(sf::Vector2f pos, sf::Texture & Texture) : Animation(pos, Texture, 0.0075, 81, 9, 9)
{
	_Sprite.setScale(sf::Vector2f(1.1f, 1.1f));
}


Explosion::~Explosion()
{
}

void Explosion::render(sf::RenderWindow & Window)
{
	if (_AnimState == Animation::Play || _AnimState == Animation::Pause)
		Window.draw(_Sprite);
}

void Explosion::update(float FrameTime)
{
	if (_AnimState == Animation::Play) {
		if (_Timer.getElapsedTime().asSeconds() > _TimePerFrame) {
			_ElapsedTime += _Timer.getElapsedTime().asSeconds();
			_Timer.restart();
			if (!nextSprite()) {
				_AnimState = Animation::Stop;
			}
		}
	}
}
